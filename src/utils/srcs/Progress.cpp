//
// Created by Stepan Usatiuk on 05.05.2023.
//

#include "../includes/Progress.h"

#include <sstream>
#include <utility>

Progress::Progress(std::function<void(std::string, int)> out, std::vector<std::variant<std::function<std::string()>, std::string>> format, const Config &conf, int level) : format(std::move(format)), out(std::move(out)), type(conf.getStr("progress")), progresslevel(level) {
    if (type != "none") {
        this->out("\n\n", level);
        thread = std::thread(&Progress::showProgress, this);
    }
}

Progress::~Progress() {
    stop = true;
    if (thread.joinable())
        thread.join();
}

void Progress::showProgress() {
    while (!stop) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            update(std::unique_lock(refreshM));
        }
    }
}

void Progress::print(const std::string &s, int level) {
    std::unique_lock refreshL(refreshM);
    out((type == "pretty" ? "\r\33[2K " : "") + s + "\n", level);
    update(std::move(refreshL));
}

void Progress::update(std::unique_lock<std::mutex> &&lock) {
    std::stringstream outs;

    if (type == "pretty")
        outs << "\r\33[2K  ";

    for (auto const &l: format) {
        if (std::holds_alternative<std::string>(l)) outs << std::get<std::string>(l);
        else
            outs << std::get<std::function<std::string()>>(l)();
    }

    if (type == "pretty")
        outs << "\r";
    else
        outs << "\n";

    out(outs.str(), progresslevel);
    lock.unlock();
}
