//
// Created by Stepan Usatiuk on 07.06.2023.
//

#ifndef BACKUP_REPOFS_H
#define BACKUP_REPOFS_H

#include <thread>

#include "Repository.h"
#include "objects/Archive.h"
#include "objects/File.h"

struct DirEntry {
    std::unordered_map<std::string, DirEntry> children;
    std::optional<File> file;
    std::string name;
    bool isFakeDir = false;
};

class RepoFS {
public:
    RepoFS(Repository *repo, Object::idType archiveId, std::string path);

    RepoFS &operator=(RepoFS rhs) = delete;
    RepoFS(const RepoFS &orig) = delete;

    ~RepoFS();

    void workerFn();
    static inline DirEntry root;
    static inline Repository *repo;

private:
    std::atomic<bool> stop = false;///< Stop flag

    Archive archive;
    std::string path;


    //    std::thread thread;///< Worker thread
};


#endif//BACKUP_REPOFS_H
