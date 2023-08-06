//
// Created by Stepan Usatiuk on 23.05.2023.
//

#ifndef SEMBACKUP_COMMANDRUN_H
#define SEMBACKUP_COMMANDRUN_H

#include "Command.h"

#include "CommandsCommon.h"

/// Runs the backup according to the config in the Repository
class CommandRun : public Command {
public:
    CommandRun();
    void run(Context ctx) override;
    static constexpr std::string_view name{"run"};

private:
    /// Internal function to chunk the file and save it
    /// \param orig     Absolute path to the file
    /// \param saveAs   UTF-8 encoded file name to save as
    /// \param callback Stats callback
    /// \return         ID of the saved file
    Object::idType backupChunkFile(const std::filesystem::path &orig, const std::string &saveAs,
                                   CommandsCommon::workerStatsFunction &callback, Context ctx);
};


#endif//SEMBACKUP_COMMANDRUN_H
