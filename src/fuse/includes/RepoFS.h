//
// Created by Stepan Usatiuk on 07.06.2023.
//

#ifndef BACKUP_REPOFS_H
#define BACKUP_REPOFS_H

#define FUSE_USE_VERSION 26

#include <memory>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>

#include "Repository.h"
#include "objects/Archive.h"
#include "objects/File.h"

struct DirEntry {
    std::unordered_map<std::string, std::unique_ptr<DirEntry>> children;
    std::optional<File> file;
    std::string name;
    bool isFakeDir = false;
};

class RepoFS {
public:
    static void start(Repository *repo, std::string path);

    static inline std::unique_ptr<DirEntry> root{std::make_unique<DirEntry>()};
    static inline Repository *repo;

    virtual ~RepoFS() = 0;
};


#endif//BACKUP_REPOFS_H
