//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "CommandRun.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#include "BytesFormatter.h"
#include "ChangeDetectorFactory.h"
#include "ChunkerFactory.h"
#include "Exception.h"
#include "Progress.h"
#include "RunningDiffAverage.h"
#include "SHA.h"
#include "Serialize.h"
#include "Signals.h"
#include "ThreadPool.h"
#include "objects/Archive.h"
#include "objects/Chunk.h"
#include "objects/File.h"

#include "CommandsCommon.h"

using namespace CommandsCommon;

CommandRun::CommandRun() : Command() {}

void CommandRun::run(Context ctx) {
    WorkerStats workerStats;///< Backup statistics of the worker threads
    RunnerStats runnerStats;///< Backup target metrics

    std::filesystem::path from = ctx.repo->getConfig().getStr("from");///< Directory to back up from

    /// Worker callback, bound to the local workerStats variable
    workerStatsFunction workerCallback = [&](unsigned long long bytesWritten, unsigned long long bytesSkipped,
                                             unsigned long long filesWritten) {
        CommandsCommon::workerCallback(bytesWritten, bytesSkipped, filesWritten, workerStats);
    };

    std::vector<Object::idType> files;///< File ids so far added to the archive
    std::mutex filesLock;             ///< Files vector lock
    /// Function to safely add new file ids to `files`
    std::function addFile = [&](Object::idType id) {
        std::lock_guard lock(filesLock);
        files.emplace_back(id);
    };

    {
        /// Calculate the average speed of backup
        RunningDiffAverage avg([&]() { return workerStats.bytesWritten.load(); }, 100, 100);

        /// Show the progress of backup
        Progress progress([this, ctx](const std::string &s, int l) { ctx.logger->write(s, l); },
                          {[&]() { return std::to_string(workerStats.filesWritten.load()); }, "/",
                           [&]() { return std::to_string(runnerStats.filesToSaveCount); }, " files saved, ",
                           [&]() { return std::to_string(runnerStats.filesSkipped); }, " files skipped, ",
                           [&]() {
                               return BytesFormatter::formatStr(
                                       (workerStats.bytesWritten.load() + workerStats.bytesSkipped.load()));
                           },
                           " / ", [&]() { return BytesFormatter::formatStr(runnerStats.bytesToSave); }, " read @ ",
                           [&]() { return BytesFormatter::formatStr(avg.get() * 10); }, "/s"},
                          ctx.repo->getConfig());

        /// Thread pool for backup tasks, prints to progress on any errors
        ThreadPool threadPool([&](const std::string &error) { progress.print("Error: " + error, 0); },
                              ctx.repo->getConfig().exists("threads") ? ctx.repo->getConfig().getInt("threads")
                                                                      : std::thread::hardware_concurrency());

        /// Container of ChangeDetectors built using the config of the repository
        ChangeDetectorContainer changeDetector = ChangeDetectorFactory::getChangeDetectors(ctx.repo->getConfig());

        /// Function to spawn a rechunking task
        auto saveFile = [&, this](const std::filesystem::path &absPath, const std::filesystem::path &relPath) {
            runnerStats.bytesToSave +=
                    File::getFileType(absPath) == File::Type::Normal ? std::filesystem::file_size(absPath) : 0;
            runnerStats.filesToSaveCount++;
            threadPool.push([&, relPath, absPath]() {
                addFile(backupChunkFile(absPath, relPath.string(), workerCallback, ctx));
                progress.print("Copied: " + relPath.string(), 1);
            });
        };

        /// Task to process an individual file in the backup
        std::function<void(std::filesystem::path)> processFile = [&, this](const std::filesystem::path &p) {
            auto relPath = p.lexically_relative(from).string();

            if (ctx.repo->exists(Object::ObjectType::File, relPath) != 0) {
                File repoFile = Serialize::deserialize<File>(ctx.repo->getObjectRaw(Object::ObjectType::File, relPath));
                if (!changeDetector.check({repoFile, ctx.repo}, {p, from})) {
                    addFile(repoFile.id);
                    progress.print("Skipped: " + relPath, 1);
                    runnerStats.filesSkipped++;
                    return;
                }
            }

            saveFile(p, relPath);
            return;
        };

        /// Start the backup with the root directory and empty ignore list
        threadPool.push([&]() {
            processDirWithIgnore(
                    from, {}, [&](std::function<void()> f) { threadPool.push(std::move(f)); }, processFile);
        });

        /// Wait for all the tasks to finish
        std::unique_lock finishedLock(threadPool.finishedLock);
        threadPool.finished.wait(finishedLock, [&threadPool] { return threadPool.empty(); });
    }

    ctx.logger->write("\n", 1);

    auto written = BytesFormatter::format(workerStats.bytesWritten);
    auto skipped = BytesFormatter::format(workerStats.bytesSkipped);

    ctx.logger->write(written.prefix + " written: " + written.number + '\n', 1);
    ctx.logger->write(skipped.prefix + " skipped: " + skipped.number + '\n', 1);

    auto time = std::time(0);
    auto ltime = std::localtime(&time);
    std::stringstream s;
    s << std::put_time(ltime, "%d-%m-%Y %H-%M-%S");
    /// Avoid archive name collisions
    while (ctx.repo->exists(Object::ObjectType::Archive, s.str())) s << "N";
    Archive a(ctx.repo->getId(), s.str(), time, files);
    ctx.repo->putObject(a);
}

Object::idType CommandRun::backupChunkFile(const std::filesystem::path &orig, const std::string &saveAs,
                                           workerStatsFunction &callback, Context ctx) {
    /// If it's a symlink or directory, treat it specially
    /// The order of checks is important, because is_directory follows the symlink
    if (std::filesystem::is_symlink(orig) || std::filesystem::is_directory(orig)) {
        auto contents = File::getFileContents(orig);
        Chunk c(ctx.repo->getId(), SHA::calculate(contents), contents);
        File f(ctx.repo->getId(), saveAs, c.length, File::getFileMtime(orig), c.SHA, {{0, c.id}},
               File::getFileType(orig));
        ctx.repo->putObject(c);
        ctx.repo->putObject(f);
        return f.id;
    }
    if (!std::filesystem::is_regular_file(orig)) throw Exception(orig.string() + "is a special file, not saving");

    std::ifstream ifstream(orig, std::ios::in | std::ios::binary);
    if (!ifstream) throw Exception("Couldn't open " + orig.string() + " for reading");
    std::unique_ptr<Chunker> chunker = ChunkerFactory::getChunker(ctx.repo->getConfig(), ifstream.rdbuf());

    SHA fileHash;

    std::map<size_t, Object::idType> fileChunks;
    unsigned long long size = 0;

    for (auto chunkp: *chunker) {
        /// Exit when asked to
        if (Signals::shouldQuit) break;

        Object::idType chunkId;
        if (ctx.repo->exists(Object::ObjectType::Chunk, chunkp.first)) {
            /// If the chunk already exists, reuse it
            chunkId = ctx.repo->getObjectId(Object::ObjectType::Chunk, chunkp.first);
            callback(0, chunkp.second.size(), 0);
        } else {
            /// Otherwise, write it
            Chunk c(ctx.repo->getId(), chunkp.first, chunkp.second);
            chunkId = c.id;
            callback(c.data.size(), 0, 0);
            ctx.repo->putObject(c);
        }
        fileHash.feedData(chunkp.second);
        fileChunks.emplace(size, chunkId);
        size += chunkp.second.size();
    }

    /// We might have exited in the loop before, so we don't save an incomplete file
    if (Signals::shouldQuit) throw Exception("Quitting!");
    if (size != File::getFileSize(orig)) {
        throw Exception("Something really bad happened or file " + orig.string() + " changed during backup");
    }
    File f(ctx.repo->getId(), saveAs, size, File::getFileMtime(orig), fileHash.getHash(), fileChunks,
           File::getFileType(orig));
    ctx.repo->putObject(f);
    callback(0, 0, 1);

    return f.id;
}
