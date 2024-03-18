
#include <iostream>

#include "BytesFormatter.h"
#include "Command.h"
#include "CommandDelete.h"
#include "CommandDiff.h"
#include "CommandList.h"
#include "CommandListFiles.h"
#include "CommandMount.h"
#include "CommandRestore.h"
#include "CommandRun.h"
#include "Config.h"
#include "Context.h"
#include "Exception.h"
#include "FileRepository.h"
#include "Logger.h"
#include "Repository.h"
#include "Serialize.h"
#include "Signals.h"
#include "objects/Archive.h"
#include "objects/File.h"

Config getConf(int argc, char *argv[]) {
    Config out;
    for (int i = 0; i < argc; i++) {
        std::string key = argv[i];
        if (key.substr(0, 2) != "--") throw Exception("Options should start with --");
        key = key.substr(2);
        if (++i == argc) throw Exception("Option not specified for " + key);
        std::string val = argv[i];
        out.add(key, val);
    }
    return out;
}

int help() {
    for (auto const &o: Config::keys) {
        std::cout << "--" << o.first << " <" << Config::KeyTypeToStr.at(o.second.type) << ">" << std::endl;
        if (o.second.defaultval.has_value()) std::cout << "  Default: " << o.second.defaultval.value() << std::endl;
        std::cout << "  Is saved in repository: " << (o.second.remember ? "yes" : "no") << std::endl;
        std::cout << "  Info: " << o.second.info << std::endl;
    }
    return 0;
}

std::unique_ptr<Repository> openRepo(Config &conf) {
    try {
        auto repo = std::make_unique<FileRepository>(conf);
        repo->open();
        return repo;
    } catch (std::exception &e) {
        std::cout << "Error opening repo: " << e.what() << std::endl;
        return nullptr;
    }
}

int init(Config &conf) {
    auto repo = std::make_unique<FileRepository>(conf);
    if (repo == nullptr) return -1;

    try {
        repo->init();
    } catch (std::exception &e) {
        std::cout << "Error initializing repo: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    try {
        Signals::setup();

        if (argc < 2) {
            std::cerr << "No argument specified" << std::endl;
            help();
            return -1;
        }

        std::string opt = argv[1];
        if (opt == "help") { return help(); }

        Config conf;

        try {
            conf = getConf(argc - 2, argv + 2);
        } catch (std::exception &e) {
            std::cerr << "Error reading config!" << std::endl << e.what() << std::endl;
            return -1;
        }

        if (opt == "init") { return init(conf); }

        auto repo = openRepo(conf);

        if (repo == nullptr) {
            std::cerr << "Can't open repo!" << std::endl;
            return -1;
        }

        Logger logger(conf.getInt("verbose"));
        Context ctx{&logger, repo.get()};

        std::unordered_map<std::string, std::unique_ptr<Command>> commands;
        commands.emplace(CommandDiff::name, std::make_unique<CommandDiff>());
        commands.emplace(CommandRestore::name, std::make_unique<CommandRestore>());
        commands.emplace(CommandRun::name, std::make_unique<CommandRun>());
        commands.emplace(CommandListFiles::name, std::make_unique<CommandListFiles>());
        commands.emplace(CommandList::name, std::make_unique<CommandList>());
        commands.emplace(CommandMount::name, std::make_unique<CommandMount>());
        commands.emplace(CommandDelete::name, std::make_unique<CommandDelete>());

        if (commands.count(opt) == 0) {
            std::cerr << "Unknown argument" << std::endl;
            return -1;
        } else {
            commands.at(opt)->run(ctx);
        }
    } catch (std::exception &e) { std::cerr << "Error!" << std::endl << e.what() << std::endl; } catch (...) {
        std::cerr << "Something very bad happened!" << std::endl;
    }
}
