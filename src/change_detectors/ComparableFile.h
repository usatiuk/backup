//
// Created by Stepan Usatiuk on 05.05.2023.
//

#ifndef SEMBACKUP_COMPARABLEFILE_H
#define SEMBACKUP_COMPARABLEFILE_H

#include <filesystem>
#include <functional>
#include <streambuf>

#include "../repo/Repository.h"
#include "../repo/objects/File.h"

/// Helper class to allow comparing files from different sources
/**
 * As we are required to allow comparisons between a File in a repository and a file in filesystem,
 * comparisons between two files that are already in a Repository,
 * and between File%s that are in a repository cache and between files in the filesystem (when making backups),
 * this helper class exists to provide a uniform interface to be used when calling ChangeDetector%s.
 */
struct ComparableFile {
    /// Constructs a ComparableFile based on a File in a Repository
    /// The resulting ComparableFile will have a #contents function that returns an instance of FileBuffer for given \p file
    /// \param file    Constant reference to a File object
    /// \param repo Constant pointer to Repository from which the File object was taken, must be valid during the lifetime of created ComparableFile
    ComparableFile(const File &file, const Repository *repo);

    /// Constructs a ComparableFile based on a file in the filesystem
    /// The resulting ComparableFile will have a #contents function that returns an instance of std::filebuf for file at given path
    /// \param p    Constant reference to an absolute path to the file
    /// \param base Constant reference to a base path against which #path will be set
    ComparableFile(const std::filesystem::path &p, const std::filesystem::path &base);

    const std::string path;                                         ///< Relative path to the file
    const File::Type type;                                          ///< File type
    const unsigned long long bytes;                                 ///< Number of bytes in the file
    const unsigned long long mtime;                                 ///< Timestamp of last file modification
    const std::function<std::unique_ptr<std::streambuf>()> contents;///< Function that returns a unique pointer to a std::streambuf instance linked to the contents of the file
};


#endif//SEMBACKUP_COMPARABLEFILE_H
