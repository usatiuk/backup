//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_FILEREPOSITORY_H
#define SEMBACKUP_FILEREPOSITORY_H

#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>

#include "Object.h"
#include "Repository.h"

/// Repository implementation in the local filesystem
/**
 * `root` Config value is used as a root
 * Objects are stored concatenated in files with approximate size of `repo-target` MB (from Config)
 * The object key/object id index is stored as a hash map, as a `index` file out of the object storage structure
 * Hints for the location of objects inside of files are also stored as a hash map in the `offsets` file
 * Config is stored in the `info` file, merged with the supplied Config on open()
 *
 * Thread safe, approx. max memory usage is `number of threads` * `repo-target`,
 * as every thread can be flushing its write cache at the same time
 */
class FileRepository final : public Repository {
public:
    /// Constructs a new FileRepository
    /// \param config   Config to use
    FileRepository(Config config);

    bool exists() override;
    bool open() override;
    bool init() override;
    bool flush() override;

    std::vector<char> getObject(Object::idType id) const override;
    bool putObject(const Object &obj) override;
    bool deleteObject(const Object &obj) override;

    std::vector<char> getObject(Object::ObjectType type, const std::string &key) const override;
    Object::idType getObjectId(Object::ObjectType type, const std::string &key) const override;
    std::vector<std::pair<std::string, Object::idType>> getObjects(Object::ObjectType type) const override;

    bool clearCache(Object::ObjectType type) override;
    bool addToCache(const Object &obj) override;

    bool exists(Object::ObjectType type, const std::string &key) const override;
    Object::idType getId() override;

    /// FileRepository destructor
    /// Flushes write cache, and writes the metadata
    ~FileRepository() override;
    FileRepository(const FileRepository &r) = delete;
    FileRepository &operator=(const FileRepository &r) = delete;

private:
    const std::filesystem::path root;///< Root of the repository in the filesystem

    /// Puts the Object raw data into write cache
    bool writeObject(const Object &obj);

    bool ready = false;/// < Indicates whether the FileRepository was open or initialized

    /// Reads the file and returns its raw data
    /// \param file  Constant reference to the absolute path of the file
    /// \return      Vector of bytes of the file
    std::vector<char> readFile(const std::filesystem::path &file) const;


    /// Reads the \psize bytes of the file from \p offset and returns its raw data
    /// \param file   Constant reference to the absolute path of the file
    /// \param offset First byte of the file to read
    /// \param size   Amount of bytes to read (no more than absoluteMaxFileLimit)
    /// \return       Vector of bytes of the file
    /// \throws       Exception on any error, or when absoluteMaxFileLimit is reached
    std::vector<char> readFile(const std::filesystem::path &file, unsigned long long offset, unsigned long long size) const;
    static constexpr unsigned long long absoluteMaxFileLimit{4ULL * 1024 * 1024 * 1024};///<Max file read size (4GB)

    /// Writes \p data to \p file
    /// \param file Constant reference to the absolute path of the file
    /// \param data Constant reference to the vector of bytes to write
    /// \return     True
    /// \throws     Exception on any error
    bool writeFile(const std::filesystem::path &file, const std::vector<char> &data);

    mutable std::mutex repoLock;///< Lock for any operations on the Repository

    /// Helper struct to store the location of objects in the filesystem
    struct OffsetEntry {
        unsigned long long fileId;///< ID of file where the object is located
        unsigned long long offset;///< Offset in the file where the object starts
        unsigned long long length;///< Length of the object
        using serializable = std::true_type;

        /// Default constructor
        OffsetEntry(unsigned long long fileId, unsigned long long offset, unsigned long long length);

        /// Deserialization constrictor
        OffsetEntry(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end);

        /// Serializes the entry to \p out
        void serialize(std::vector<char> &out) const;
    };

    unsigned long long maxFileId = 1;                           ///< Largest ID of object storage file
    std::unordered_map<Object::idType, OffsetEntry> offsetIndex;///< Used to locate Object%s in the filesystem

    std::mutex writeCacheLock;                             ///< Write cache lock
    std::map<Object::idType, std::vector<char>> writeCache;///< Write cache, map of Object ids and their serialized data
    unsigned long long writeCacheSize = 0;                 ///< Current byte size of the write cache
    const unsigned long long writeCacheMax;                ///< Target size of the write cache, it is automatically flushed after this is reached

    /// Flushes the write cache
    /// Takes the cache lock, swaps the cache with an empty one and unlocks it
    /// \param lockW Write cache lock
    void flushWriteCache(std::unique_lock<std::mutex> &&lockW);

    Object::idType largestUnusedId = 1;                                                              ///< Largest available objectID
    std::unordered_map<Object::ObjectType, std::unordered_map<std::string, Object::idType>> keyIndex;///< Maps Object%'s keys to their ID's
};


#endif//SEMBACKUP_FILEREPOSITORY_H
