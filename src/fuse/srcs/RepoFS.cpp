//
// Created by Stepan Usatiuk on 07.06.2023.
//


#include "RepoFS.h"

#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <fuse.h>
#include <iostream>

#include "Serialize.h"
#include "objects/Chunk.h"

DirEntry *getf(std::string path) {
    auto p = std::filesystem::relative(std::filesystem::path(path), "/");
    DirEntry *entry = RepoFS::root.get();
    if (p != ".")
        for (auto const &subp: p) { entry = entry->children.at(subp).get(); }
    return entry;
}

static int rfsGetattr(const char *path, struct stat *stbuf) {
    int res = 0;

    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return res;
    }
    DirEntry *e;
    try {
        e = getf(path);
        if (e->isFakeDir || e->file->fileType == File::Type::Directory) {
            stbuf->st_mode = S_IFDIR | 0755;
            stbuf->st_nlink = 2;
        } else if (e->file->fileType == File::Type::Normal) {
            stbuf->st_mode = S_IFREG | 0444;
            stbuf->st_nlink = 1;
            stbuf->st_size = e->file->bytes;
        } else if (e->file->fileType == File::Type::Symlink) {
            stbuf->st_mode = S_IFLNK | 0777;
            stbuf->st_nlink = 1;
            stbuf->st_size = e->file->bytes;
        }
    } catch (...) { return -ENOENT; }

    return res;
}

static int rfsReaddir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
    (void) offset;
    (void) fi;

    DirEntry *entry = RepoFS::root.get();
    if (std::string(path) != "/") try {
            entry = getf(path);
        } catch (...) { return -ENOENT; }

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);

    for (auto const &e: entry->children) {
        auto pstr = e.second->name;
        filler(buf, pstr.c_str(), NULL, 0);
    }

    return 0;
}

static int rfsOpen(const char *path, struct fuse_file_info *fi) {
    DirEntry *entry = RepoFS::root.get();
    if (std::string(path) != "/") try {
            entry = getf(path);
        } catch (...) { return -ENOENT; }

    if ((fi->flags & 3) != O_RDONLY) return -EACCES;

    return 0;
}

static int rfsRead(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    size_t len;
    (void) fi;
    DirEntry *entry = RepoFS::root.get();
    if (std::string(path) != "/") try {
            entry = getf(path);
        } catch (...) { return -ENOENT; }


    len = entry->file->bytes;
    if (offset < len) {
        if (offset + size > len) size = len - offset;

        auto curchunk = entry->file->chunks.upper_bound(offset);
        if (curchunk == entry->file->chunks.begin()) { std::abort(); }
        --curchunk;
        size_t curInBuf = 0;
        size_t curInChunk = offset - curchunk->first;
        while (curInBuf < size) {
            auto chunk = Serialize::deserialize<Chunk>(RepoFS::repo->getObject(curchunk->second));
            size_t read = std::min((size_t) chunk.length - curInChunk, size - curInBuf);
            memcpy(buf + curInBuf, chunk.data.data() + curInChunk, read);
            curInBuf += read;
            curInChunk = 0;
            ++curchunk;
        }
    } else
        size = 0;

    return size;
}

static int rfsReadlink(const char *path, char *buf, size_t size) {
    DirEntry *entry = RepoFS::root.get();
    if (std::string(path) != "/") try {
            entry = getf(path);
        } catch (...) { return -ENOENT; }

    if (entry->file->fileType != File::Type::Symlink) return -ENOENT;
    auto dst = Serialize::deserialize<Chunk>(RepoFS::repo->getObject(entry->file->chunks.at(0)));
    strncpy(buf, dst.data.data(), std::min(dst.data.size(), size));

    return 0;
}

static struct fuse_operations rfsOps = {
        .getattr = rfsGetattr,
        .readlink = rfsReadlink,
        .open = rfsOpen,
        .read = rfsRead,
        .readdir = rfsReaddir,
};

void RepoFS::start(Repository *repo, std::string path) {
    RepoFS::repo = repo;
    auto ars = repo->getObjects(Object::ObjectType::Archive);
    for (auto const &r: ars) {
        auto a = Serialize::deserialize<Archive>(repo->getObject(r.second));
        for (auto const &f: a.files) {
            auto file = Serialize::deserialize<File>(repo->getObject(f));
            auto path = std::filesystem::path(file.name);
            DirEntry *entry = root->children[std::to_string(a.id)].get()
                                      ? root->children[std::to_string(a.id)].get()
                                      : (root->children[std::to_string(a.id)] = std::make_unique<DirEntry>()).get();
            entry->isFakeDir = true;
            entry->name = std::to_string(a.id);
            for (auto const &subp: path) {
                entry = entry->children[subp].get() ? entry->children[subp].get()
                                                    : (entry->children[subp] = std::make_unique<DirEntry>()).get();
            }
            entry->file.emplace(file);
            entry->name = std::filesystem::path(file.name).filename().string();
        }
    }


    int argc = 3;
    char *argv[] = {"", "-f", const_cast<char *>(path.c_str())};
    std::cout << static_cast<int>(fuse_main(argc, argv, &rfsOps, nullptr));
}
