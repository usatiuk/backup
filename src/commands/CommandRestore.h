//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMANDRESTORE_H
#define SEMBACKUP_COMMANDRESTORE_H

#include "Command.h"

#include "../repo/objects/File.h"

#include "CommandsCommon.h"

/// Restores the archive with id \aid to path \p to (from config)
class CommandRestore : public Command {
public:
    CommandRestore();
    void run(Context ctx) override;

private:
    /// Internal function to restore a file
    /// \param file     Constant reference to the File object
    /// \param base     Base directory to restore to
    /// \param callback Stats callback
    /// \return         Name of the restored file
    std::string backupRestoreFile(const File &file, const std::filesystem::path &base, CommandsCommon::workerStatsFunction &callback, Context ctx);
};


#endif//SEMBACKUP_COMMANDRESTORE_H
