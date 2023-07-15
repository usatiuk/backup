//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "CommandRestore.h"

#include <fstream>
#include <sstream>

#include "BytesFormatter.h"
#include "ChunkerFactory.h"
#include "Exception.h"
#include "Progress.h"
#include "RunningDiffAverage.h"
#include "Serialize.h"
#include "Signals.h"
#include "ThreadPool.h"
#include "objects/Archive.h"
#include "objects/Chunk.h"

using namespace CommandsCommon;

CommandRestore::CommandRestore() : Command() {
}

void CommandRestore::run(Context ctx) {
    Object::idType archive = ctx.repo->getConfig().getInt("aid");
    std::filesystem::path to = std::filesystem::u8path(ctx.repo->getConfig().getStr("to"));

    std::atomic<unsigned long long> filesToRestoreCount = 0;
    std::atomic<unsigned long long> bytesToRestore = 0;

    WorkerStats workerStats;///< Backup statistics of the worker threads

    /// Worker callback, bound to the local workerStats variable
    workerStatsFunction workerCallback = [&workerStats](unsigned long long bytesWritten, unsigned long long bytesSkipped, unsigned long long filesWritten) {
        CommandsCommon::workerCallback(bytesWritten, bytesSkipped, filesWritten, workerStats);
    };
    {
        /// Calculate the average speed of backup
        RunningDiffAverage avg(
                [&]() { return workerStats.bytesWritten.load(); },
                100, 100);

        /// Show restore progress
        Progress progress([this, ctx](const std::string &s, int l) { ctx.logger->write(s, l); },
                          {
                                  [&workerStats]() { return std::to_string(workerStats.filesWritten.load()); },
                                  "/",
                                  [&filesToRestoreCount]() { return std::to_string(filesToRestoreCount); },
                                  " files saved, ",
                                  [&workerStats]() { return BytesFormatter::formatStr(workerStats.bytesWritten.load() + workerStats.bytesSkipped.load()); },
                                  " / ",
                                  [&bytesToRestore]() { return BytesFormatter::formatStr(bytesToRestore); },
                                  " saved @ ",
                                  [&avg]() { return BytesFormatter::formatStr(avg.get() * 10); },
                                  "/s",
                          },
                          ctx.repo->getConfig());

        /// Thread pool for restore tasks
        ThreadPool threadPool([&](const std::string &error) {
            progress.print("Error: " + error, 0);
        },
                              ctx.repo->getConfig().exists("threads") ? ctx.repo->getConfig().getInt("threads") : std::thread::hardware_concurrency());

        /// Add the main restore task
        threadPool.push([&, this]() {
            /// Get the archive and its file IDs
            auto archiveO = Serialize::deserialize<Archive>(ctx.repo->getObject(archive));
            std::vector<Object::idType> files = archiveO.files;
            /// For each file...
            for (const auto fid: files) {
                /// Stop when asked to
                if (Signals::shouldQuit) break;

                auto file = Serialize::deserialize<File>(ctx.repo->getObject(fid));
                filesToRestoreCount++;
                bytesToRestore += file.bytes;
                /// Spawn a restore task
                threadPool.push([&, this, to, file]() {
                    backupRestoreFile(file, to, workerCallback, ctx);
                    progress.print("Restored " + file.name, 1);
                });
            }
        });

        /// Wait for all tasks to finish
        std::unique_lock finishedLock(threadPool.finishedLock);
        threadPool.finished.wait(finishedLock, [&threadPool] { return threadPool.empty(); });
    }
    ctx.logger->write("\n", 1);
}

std::string CommandRestore::backupRestoreFile(const File &file, const std::filesystem::path &baseDir, workerStatsFunction &callback, Context ctx) {
    auto fullpath = baseDir / std::filesystem::u8path(file.name);

    std::filesystem::create_directories(fullpath.parent_path());

    if (file.fileType == File::Type::Directory) {
        std::filesystem::create_directory(fullpath);
        callback(0, 0, 1);
        return fullpath.u8string();
    }
    if (file.fileType == File::Type::Symlink) {
        auto dest = Serialize::deserialize<Chunk>(ctx.repo->getObject(file.chunks.at(0)));
        std::filesystem::create_symlink(std::filesystem::u8path(std::string{dest.data.begin(), dest.data.end()}), fullpath);
        callback(0, 0, 1);
        return fullpath.u8string();
    }

    std::ofstream ostream(fullpath, std::ios::binary | std::ios::out | std::ios::trunc);
    for (const auto cid: file.chunks) {
        if (Signals::shouldQuit) throw Exception("Quitting!");

        Chunk c = Serialize::deserialize<Chunk>(ctx.repo->getObject(cid.second));
        if (!c.data.empty()) {
            ostream.rdbuf()->sputn(c.data.data(), c.data.size());
            callback(c.data.size(), 0, 0);
        }
    }
    callback(0, 0, 1);

    return fullpath.u8string();
}
