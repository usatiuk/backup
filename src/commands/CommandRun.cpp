//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "CommandRun.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#include "../BytesFormatter.h"
#include "../Exception.h"
#include "../Progress.h"
#include "../RunningDiffAverage.h"
#include "../Signals.h"
#include "../ThreadPool.h"
#include "../change_detectors/ChangeDetectorFactory.h"
#include "../chunkers/ChunkerFactory.h"
#include "../crypto/MD5.h"
#include "../repo/Serialize.h"
#include "../repo/objects/Archive.h"
#include "../repo/objects/Chunk.h"
#include "../repo/objects/File.h"

#include "CommandsCommon.h"

using namespace CommandsCommon;

CommandRun::CommandRun() : Command("run") {
}

void CommandRun::run(Context ctx) {
    WorkerStats workerStats;///< Backup statistics of the worker threads
    RunnerStats runnerStats;///< Backup target metrics

    std::filesystem::path from = ctx.repo->getConfig().getStr("from");///< Directory to back up from
    bool fullBackup = ctx.repo->getConfig().getStr("type") == "full";
    if (fullBackup) {
        ctx.logger->write("Backup is full because of the config\n", 1);
    }
    /// For progtest task compliance
    if (!fullBackup) {
        /// If it's time for full backup as per config, force it
        auto per = ctx.repo->getConfig().getInt("full-period");
        auto list = ctx.repo->getObjects(Object::ObjectType::Archive);
        std::sort(list.begin(), list.end(), [](const auto &l, const auto &r) { return l.second > r.second; });
        int lastInc = 0;
        for (auto const &a: list) {
            auto archiveO = Serialize::deserialize<Archive>(ctx.repo->getObject(a.second));
            if (!archiveO.isFull) {
                lastInc++;
                continue;
            } else
                break;
        }
        if (lastInc >= per) {
            fullBackup = true;
            ctx.logger->write("Backup is full because of the interval\n", 1);
        }
        if (list.size() == 0) {
            fullBackup = true;
            ctx.logger->write("Backup is full because there are no backups\n", 1);
        }
    }

    /// Worker callback, bound to the local workerStats variable
    workerStatsFunction workerCallback = [&](unsigned long long bytesWritten, unsigned long long bytesSkipped, unsigned long long filesWritten) {
        CommandsCommon::workerCallback(bytesWritten, bytesSkipped, filesWritten, workerStats);
    };

    std::vector<Object::idType> files;///< File ids so far added to the archive
    std::mutex filesLock;             ///< Files vector lock
    /// Function to safely add new file ids to `files`
    std::function addFile = [&](Object::idType id) {std::lock_guard lock(filesLock); files.emplace_back(id); };

    /// Technically the progtest task says that only the files from the last backup should be compared against...
    std::map<std::string, Object::idType> prevArchiveFiles;
    {
        auto prevArchiveFilesList = ctx.repo->getObjects(Object::ObjectType::File);
        prevArchiveFiles = {prevArchiveFilesList.begin(), prevArchiveFilesList.end()};
    }
    ctx.repo->clearCache(Object::ObjectType::File);

    {
        /// Calculate the average speed of backup
        RunningDiffAverage avg(
                [&]() { return workerStats.bytesWritten.load(); },
                100, 100);

        /// Show the progress of backup
        Progress progress([this, ctx](const std::string &s, int l) { ctx.logger->write(s, l); },
                          {[&]() { return std::to_string(workerStats.filesWritten.load()); },
                           "/",
                           [&]() { return std::to_string(runnerStats.filesToSaveCount); },
                           " files saved, ",
                           [&]() { return std::to_string(runnerStats.filesSkipped); },
                           " files skipped, ",
                           [&]() { return BytesFormatter::formatStr((workerStats.bytesWritten.load() + workerStats.bytesSkipped.load())); },
                           " / ",
                           [&]() { return BytesFormatter::formatStr(runnerStats.bytesToSave); },
                           " read @ ",
                           [&]() { return BytesFormatter::formatStr(avg.get() * 10); },
                           "/s"},
                          ctx.repo->getConfig());

        /// Thread pool for backup tasks, prints to progress on any errors
        ThreadPool threadPool([&](const std::string &error) {
            progress.print("Error: " + error, 0);
        },
                              ctx.repo->getConfig().exists("threads") ? ctx.repo->getConfig().getInt("threads") : std::thread::hardware_concurrency());

        /// Container of ChangeDetectors built using the config of the repository
        ChangeDetectorContainer changeDetector = ChangeDetectorFactory::getChangeDetectors(ctx.repo->getConfig());

        /// Function to spawn a rechunking task
        auto saveFile = [&, this](const std::filesystem::path &absPath, const std::filesystem::path &relPath) {
            runnerStats.bytesToSave += File::getFileType(absPath) == File::Type::Normal ? std::filesystem::file_size(absPath) : 0;
            runnerStats.filesToSaveCount++;
            threadPool.push([&, relPath, absPath]() {
                addFile(backupChunkFile(absPath, relPath.u8string(), workerCallback, ctx));
                progress.print("Copied: " + relPath.u8string(), 1);
            });
        };

        /// Task to process an individual file in the backup
        std::function<void(std::filesystem::path)> processFile;
        /// If it's a full backup, just save the file, otherwise re-chunk it only if it's changed
        if (fullBackup)
            processFile =
                    [&, this](const std::filesystem::path &p) {
                        saveFile(p, p.lexically_relative(from).u8string());
                    };
        else
            processFile =
                    [&, this](const std::filesystem::path &p) {
                        auto relPath = p.lexically_relative(from).u8string();

                        if (prevArchiveFiles.count(relPath) != 0) {
                            File repoFile = Serialize::deserialize<File>(ctx.repo->getObject(prevArchiveFiles.at(relPath)));
                            if (!changeDetector.check({repoFile, ctx.repo}, {p, from})) {
                                addFile(repoFile.id);
                                ctx.repo->addToCache(repoFile);
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
                    from,
                    {},
                    [&](std::function<void()> f) { threadPool.push(std::move(f)); },
                    processFile);
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
    Archive a(ctx.repo->getId(), s.str(), time, files, fullBackup);
    ctx.repo->putObject(a);
}

Object::idType CommandRun::backupChunkFile(const std::filesystem::path &orig, const std::string &saveAs, workerStatsFunction &callback, Context ctx) {
    /// If it's a symlink or directory, treat it specially
    /// The order of checks is important, because is_directory follows the symlink
    if (std::filesystem::is_symlink(orig) || std::filesystem::is_directory(orig)) {
        auto contents = File::getFileContents(orig);
        Chunk c(ctx.repo->getId(), MD5::calculate(contents), contents);
        File f(ctx.repo->getId(), saveAs, c.length, File::getFileMtime(orig), c.md5, {c.id}, File::getFileType(orig));
        ctx.repo->putObject(c);
        ctx.repo->putObject(f);
        return f.id;
    }
    if (!std::filesystem::is_regular_file(orig))
        throw Exception(orig.u8string() + "is a special file, not saving");

    std::ifstream ifstream(orig, std::ios::in | std::ios::binary);
    if (!ifstream) throw Exception("Couldn't open " + orig.u8string() + " for reading");
    std::unique_ptr<Chunker> chunker = ChunkerFactory::getChunker(ctx.repo->getConfig(), ifstream.rdbuf());

    MD5 fileHash;

    std::vector<Object::idType> fileChunks;
    unsigned long long size = 0;

    for (auto chunkp: *chunker) {
        /// Exit when asked to
        if (Signals::shouldQuit) break;

        Object::idType chunkId;
        size += chunkp.second.size();
        if (ctx.repo->getConfig().getStr("dedup") == "on" && ctx.repo->exists(Object::ObjectType::Chunk, chunkp.first)) {
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
        fileChunks.emplace_back(chunkId);
    }

    /// We might have exited in the loop before, so we don't save an incomplete file
    if (Signals::shouldQuit) throw Exception("Quitting!");
    if (size != File::getFileSize(orig)) {
        throw Exception("Something really bad happened or file " + orig.u8string() + " changed during backup");
    }
    File f(ctx.repo->getId(), saveAs, size, File::getFileMtime(orig), fileHash.getHash(), fileChunks, File::getFileType(orig));
    ctx.repo->putObject(f);
    callback(0, 0, 1);

    return f.id;
}
