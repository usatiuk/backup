//
// Created by Stepan Usatiuk on 23.05.2023.
//

#include "../includes/CommandsCommon.h"

#include <fstream>
#include <regex>

#include "../../utils/includes/Exception.h"
#include "../../utils/includes/Signals.h"

void CommandsCommon::workerCallback(unsigned long long int bytesWritten, unsigned long long int bytesSkipped, unsigned long long int filesWritten, WorkerStats &to) {
    to.bytesWritten += bytesWritten;
    to.bytesSkipped += bytesSkipped;
    to.filesWritten += filesWritten;
}

bool CommandsCommon::isSubpath(const std::filesystem::path &prefix, const std::filesystem::path &p) {
    if (prefix.u8string().size() > p.u8string().size()) return false;
    for (int i = 0; i < prefix.u8string().size(); i++)
        if (p.u8string()[i] != prefix.u8string()[i]) return false;
    return true;
}

void CommandsCommon::processDirWithIgnore(const std::filesystem::path &dir, std::vector<std::string> ignore, const std::function<void(std::function<void()>)> &spawner, std::function<void(std::filesystem::directory_entry)> processFile) {
    if (!std::filesystem::is_directory(dir)) throw Exception(dir.u8string() + " is not a directory!");

    /// Don't process the directory if it has a ".nobackup" file
    if (std::filesystem::exists(dir / ".nobackup")) return;

    /// If it has an .ignore file, add every line of it into our ignore vector
    if (std::filesystem::exists(dir / ".ignore")) {
        std::ifstream ignorefile(dir / ".ignore", std::ios::in);
        std::string line;
        while (std::getline(ignorefile, line)) {
            ignore.emplace_back(line);
        }
    }

    /// For each directory entry...
    for (const auto &dirEntry: std::filesystem::directory_iterator(dir)) {
        /// Break in case exit was requested by the user
        if (Signals::shouldQuit) break;

        /// Don't process the entry if it matches any of the ignore rules
        if (std::any_of(ignore.begin(), ignore.end(), [dirEntry](auto pred) {
                std::smatch m;
                auto s = dirEntry.path().filename().u8string();
                return std::regex_match(s, m, std::regex(pred));
            })) continue;

        /// If it's a directory, spawn a task to process the entries in it
        if (!dirEntry.is_symlink() && dirEntry.is_directory()) {
            spawner([dirEntry, ignore, spawner, processFile]() {
                processDirWithIgnore(dirEntry.path(), ignore, spawner, processFile);
            });
            /// Don't save the dir if it has a .nobackup file
            if (std::filesystem::exists(dirEntry.path() / ".nobackup")) continue;
        }

        /// Spawn a task to process each individual file
        spawner([processFile, dirEntry]() {
            processFile(dirEntry);
        });
    }
}