//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMANDSCOMMON_H
#define SEMBACKUP_COMMANDSCOMMON_H

#include <atomic>
#include <filesystem>
#include <functional>

namespace CommandsCommon {
    // Bytes written, bytes skipped, files written
    using workerStatsFunction = std::function<void(unsigned long long, unsigned long long, unsigned long long)>;

    /// Internat function for recursive directory processing, taking into account ".ignore" and ".nobackup" files
    /// \param dir          Const reference to the path of directory to iterate through
    /// \param ignore       List of files to ignore
    /// \param spawner      Function to spawn other tasks
    /// \param processFile  Task to spawn on found files
    void processDirWithIgnore(const std::filesystem::path &dir, std::vector<std::string> ignore, const std::function<void(std::function<void()>)> &spawner, std::function<void(std::filesystem::directory_entry)> processFile);

    struct WorkerStats {
    public:
        std::atomic<unsigned long long> bytesWritten = 0;
        std::atomic<unsigned long long> bytesSkipped = 0;
        std::atomic<unsigned long long> filesWritten = 0;
    };

    struct RunnerStats {
    public:
        std::atomic<unsigned long long> bytesToSave = 0;
        std::atomic<unsigned long long> filesToSaveCount = 0;
        std::atomic<unsigned long long> filesSkipped = 0;
    };

    /// Checks if \p p has \p prefix as prefix
    /// \param prefix Constant reference to the prefix path
    /// \param p      Constant reference to the checked path
    /// \return       True if \p p contains \p prefix at its prefix, False otherwise
    bool isSubpath(const std::filesystem::path &prefix, const std::filesystem::path &p);

    void workerCallback(unsigned long long bytesWritten, unsigned long long bytesSkipped, unsigned long long filesWritten, WorkerStats &to);

};// namespace CommandsCommon


#endif//SEMBACKUP_COMMANDSCOMMON_H
