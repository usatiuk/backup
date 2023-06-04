//
// Created by Stepan Usatiuk on 05.05.2023.
//

#ifndef SEMBACKUP_PROGRESS_H
#define SEMBACKUP_PROGRESS_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <variant>

#include "Config.h"

/// Class to handle writing progress to the screen
class Progress {
public:
    /// Constructs the Progress instance
    /// \param out      Function to call for output
    /// \param format   Format of the progress string, vector of strings or functions that return strings
    /// \param conf     Config, used to specify format (`pretty` for line rewriting, `simple` for normal line printing, or `none`)
    Progress(std::function<void(std::string, int)> out, std::vector<std::variant<std::function<std::string()>, std::string>> format, const Config &conf, int level = 1);

    Progress &operator=(Progress rhs) = delete;
    Progress(const Progress &orig) = delete;

    /// Write a string to the terminal without disturbing the progress bar
    void print(const std::string &s, int level);

    /// Destructor, instructs the worker thread to stop
    ~Progress();

private:
    int progresslevel;
    std::vector<std::variant<std::function<std::string()>, std::string>> format;///< Format of the progressbar
    std::function<void(std::string, int)> out;                                  ///< Output function

    /// Thread loop function
    void showProgress();
    std::atomic<bool> stop = false;///< Stop flag

    std::mutex refreshM;///< Used to prevent mangling the output between print and progressbar update

    /// Prints the progressbar on screen, then unlocks the mutex
    void update(std::unique_lock<std::mutex> &&lock);
    const std::string type;///< Progressbar type (Taken from Config)

    std::thread thread;///< Worker thread
};


#endif//SEMBACKUP_PROGRESS_H
