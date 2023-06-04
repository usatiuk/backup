//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "../includes/CommandDiff.h"

#include "../../change_detectors/includes/ChangeDetectorFactory.h"
#include "../../chunkers/includes/ChunkerFactory.h"
#include "../../repo/includes/Serialize.h"
#include "../../repo/includes/objects/Archive.h"
#include "../../repo/includes/objects/Chunk.h"
#include "../../utils/includes/BytesFormatter.h"
#include "../../utils/includes/Exception.h"
#include "../../utils/includes/Progress.h"
#include "../../utils/includes/RunningDiffAverage.h"
#include "../../utils/includes/Signals.h"
#include "../../utils/includes/ThreadPool.h"
#include "../includes/Diff.h"

using namespace CommandsCommon;

CommandDiff::CommandDiff() : Command("diff") {}

void CommandDiff::run(Context ctx) {
    std::string diffMode = ctx.repo->getConfig().getStr("diff-mode");

    Object::idType archive1;
    if (!ctx.repo->getConfig().exists("aid")) {
        auto archives = ctx.repo->getObjects(Object::ObjectType::Archive);
        archive1 = std::max_element(archives.begin(), archives.end(), [](const auto &a1, const auto &a2) { return a1.second < a2.second; })->second;
    } else {
        archive1 = ctx.repo->getConfig().getInt("aid");
    }

    ThreadPool threadPool([&](const std::string &error) {
        ctx.logger->write("Error: " + error, 0);
    },
                          ctx.repo->getConfig().exists("threads") ? ctx.repo->getConfig().getInt("threads") : std::thread::hardware_concurrency());

    auto archiveO1 = Serialize::deserialize<Archive>(ctx.repo->getObject(archive1));
    std::mutex filesLock;
    std::map<std::filesystem::path, File> files;///< Files in the first archive
    for (auto id: archiveO1.files) {
        auto file = Serialize::deserialize<File>(ctx.repo->getObject(id));
        auto path = std::filesystem::u8path(file.name);
        if (isSubpath(ctx.repo->getConfig().getStr("prefix"), path))
            files.emplace(file.getKey(), std::move(file));
    }

    /// Container of ChangeDetectors built using the config of the repository
    ChangeDetectorContainer changeDetector = ChangeDetectorFactory::getChangeDetectors(ctx.repo->getConfig());

    /// Task to to compare the given file with the first archive
    auto processFile = [&, this](ComparableFile p) {
        auto relPath = p.path;
        std::unique_lock lock(filesLock);
        if (files.count(relPath) == 0) {
            ctx.logger->write(relPath + " is new\n", 0);
            lock.unlock();
        } else {
            File repoFile = files.at(relPath);
            lock.unlock();
            if (changeDetector.check({repoFile, ctx.repo}, p)) {
                ctx.logger->write(relPath + " is different " + Diff::diff({repoFile, ctx.repo}, p) + "\n", 1);
            } else {
                if (diffMode == "file")
                    ctx.logger->write(relPath + " are same ", 0);
            }
        }

        lock.lock();
        files.erase(relPath);
    };

    std::optional<Archive> archiveO2;
    if (diffMode == "normal") {
        /// If a second archive is given, run the task for each of its files, otherwise use the "from" config option
        if (ctx.repo->getConfig().exists("aid2")) {
            archiveO2.emplace(Serialize::deserialize<Archive>(ctx.repo->getObject(ctx.repo->getConfig().getInt("aid2"))));

            threadPool.push([&]() {
                for (auto id: archiveO2.value().files) {
                    /// Exit when asked to
                    if (Signals::shouldQuit) throw Exception("Quitting");
                    auto file = Serialize::deserialize<File>(ctx.repo->getObject(id));
                    if (isSubpath(ctx.repo->getConfig().getStr("prefix"), std::filesystem::u8path(file.name)))
                        threadPool.push([&, file]() {
                            processFile(ComparableFile{file, ctx.repo});
                        });
                    if (Signals::shouldQuit) break;
                }

                return true;
            });
        } else {
            std::filesystem::path from = ctx.repo->getConfig().getStr("from");
            /// Start the diff with the root directory and empty ignore list
            threadPool.push([&, from]() {
                processDirWithIgnore(
                        from,
                        {},
                        [&](std::function<void()> f) { threadPool.push(std::move(f)); },
                        [processFile, from, prefix = ctx.repo->getConfig().getStr("prefix")](const std::filesystem::directory_entry &dirEntry) {
                            if (isSubpath(prefix, dirEntry.path().lexically_relative(from)))
                                processFile(ComparableFile{dirEntry, from});
                        });
            });
        }
    } else if (diffMode == "file") {
        if (files.count(ctx.repo->getConfig().getStr("prefix")) == 0) {
            ctx.logger->write("Doesn't exist in the first archive", 0);
            return;
        }

        if (ctx.repo->getConfig().exists("aid2")) {
            archiveO2.emplace(Serialize::deserialize<Archive>(ctx.repo->getObject(ctx.repo->getConfig().getInt("aid2"))));
            std::map<std::filesystem::path, File> files2;///< Files in the first archive
            for (auto id: archiveO2->files) {
                auto file = Serialize::deserialize<File>(ctx.repo->getObject(id));
                auto path = std::filesystem::u8path(file.name);
                if (isSubpath(ctx.repo->getConfig().getStr("prefix"), path))
                    files2.emplace(file.getKey(), std::move(file));
            }

            if (files2.count(ctx.repo->getConfig().getStr("prefix")) == 0) {
                ctx.logger->write("Doesn't exist in the second archive", 0);
                return;
            } else {
                processFile(ComparableFile{files2.at(ctx.repo->getConfig().getStr("prefix")), ctx.repo});
            }
        } else {
            std::filesystem::path from = ctx.repo->getConfig().getStr("from");
            if (!std::filesystem::exists(from / ctx.repo->getConfig().getStr("prefix"))) {
                ctx.logger->write("Doesn't exist in the filesystem archive", 0);
                return;
            }
            /// Start the diff with the root directory and empty ignore list
            processFile(ComparableFile{from / ctx.repo->getConfig().getStr("prefix"), from});
        }

    } else {
        throw Exception("Unknown diff-mode: " + diffMode);
    }

    /// Wait for diff to end
    std::unique_lock finishedLock(threadPool.finishedLock);
    threadPool.finished.wait(finishedLock, [&threadPool] { return threadPool.empty(); });
    if (diffMode == "normal")
        for (auto const &s: files) {
            ctx.logger->write(s.first.u8string() + " is removed\n", 0);
        }
}
