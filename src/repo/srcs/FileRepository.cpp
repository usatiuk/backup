//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "FileRepository.h"

#include <exception>
#include <iterator>
#include <mutex>

#include "CheckFilter.h"
#include "FilterFactory.h"
#include "Object.h"
#include "Serialize.h"

FileRepository::FileRepository(Config config)
    : Repository(std::move(config)), root(std::filesystem::path(this->config.getStr("repo"))),
      writeCacheMax(this->config.getInt("repo-target") * 1024 * 1024) {}

bool FileRepository::exists() { return std::filesystem::is_directory(root) && std::filesystem::exists(root / "info"); }

bool FileRepository::flush() {
    flushWriteCache(std::unique_lock(writeCacheLock));
    return true;
}

bool FileRepository::open() {
    if (!exists()) throw Exception("Repository doesn't exist!");

    auto readConf = Serialize::deserialize<Config>(CheckFilter::filterReadStatic(readFile(root / "info")));
    std::swap(config, readConf);
    config.merge(readConf);

    if (config.getStr("compression") != "none")
        filters.addFilter(FilterFactory::makeFilter(config.getStr("compression"), config));
    if (config.getStr("encryption") != "none")
        filters.addFilter(FilterFactory::makeFilter(config.getStr("encryption"), config));
    filters.addFilter(FilterFactory::makeFilter("crc", config));

    ready = true;
    try {
        std::tie(maxFileId, offsetIndex) =
                Serialize::deserialize<std::pair<decltype(maxFileId), decltype(offsetIndex)>>(
                        filters.filterRead(readFile(root / "offsets")));
        std::tie(keyIndex, largestUnusedId) =
                Serialize::deserialize<std::pair<decltype(keyIndex), decltype(largestUnusedId)>>(
                        filters.filterRead(readFile(root / "index")));
    } catch (const std::exception &e) {
        ready = false;
        throw;
    }

    return true;
}

bool FileRepository::init() {
    if (ready) throw Exception("Trying to initialize already initialized repository!");
    if (exists()) throw Exception("Trying to initialize already existing repository!");

    if (!std::filesystem::is_directory(root) && !std::filesystem::create_directories(root))
        throw Exception("Can't create directory " + root.string());

    writeFile(root / "info", CheckFilter::filterWriteStatic(Serialize::serialize(config)));

    if (config.getStr("compression") != "none")
        filters.addFilter(FilterFactory::makeFilter(config.getStr("compression"), config));
    if (config.getStr("encryption") != "none")
        filters.addFilter(FilterFactory::makeFilter(config.getStr("encryption"), config));
    filters.addFilter(FilterFactory::makeFilter("crc", config));

    ready = true;
    return true;
}

FileRepository::~FileRepository() {
    if (ready) {
        ready = false;
        flushWriteCache(std::unique_lock(writeCacheLock));

        writeFile(root / "offsets", filters.filterWrite(Serialize::serialize(std::make_pair(maxFileId, offsetIndex))));
        writeFile(root / "index", filters.filterWrite(Serialize::serialize(std::make_pair(keyIndex, largestUnusedId))));
    }
}

std::vector<char> FileRepository::getObjectRaw(Object::idType id) const {
    if (!ready) throw Exception("Tried working with uninitialized repo!");

    std::unique_lock lock(repoLock);
    if (offsetIndex.count(id) == 0) throw Exception("Object with id " + std::to_string(id) + " doesn't exist!");
    auto entry = offsetIndex.at(id);
    lock.unlock();

    return filters.filterRead(readFile(root / std::to_string(entry.fileId), entry.offset, entry.length));
}

bool FileRepository::writeObject(const Object &obj) {
    if (!ready) throw Exception("Tried working with uninitialized repo!");
    auto tmp = filters.filterWrite(Serialize::serialize(obj));
    {
        std::unique_lock lockW(writeCacheLock);
        writeCacheSize += tmp.size();
        writeCache[obj.id] = std::move(tmp);

        // If we have reached the target file size, flush the cache
        if (writeCacheSize >= writeCacheMax) { flushWriteCache(std::move(lockW)); }
    }
    return true;
}

void FileRepository::flushWriteCache(std::unique_lock<std::mutex> &&lockW) {
    if (writeCache.empty()) {
        lockW.unlock();
        return;
    }

    // Swap the cache for a new one and unlock the mutex so other threads can continue working
    decltype(writeCache) objs;
    std::swap(writeCache, objs);
    writeCacheSize = 0;

    decltype(maxFileId) currentFileId;
    {
        std::lock_guard lockI(repoLock);
        currentFileId = maxFileId;
        maxFileId++;
    }
    lockW.unlock();

    unsigned long long offset = 0;
    std::ofstream ofstream(root / std::to_string(currentFileId), std::ios::binary | std::ios::trunc | std::ios::out);

    for (auto &i: objs) {
        {
            std::lock_guard lockI(repoLock);
            offsetIndex.emplace(i.first, OffsetEntry(currentFileId, offset, i.second.size()));
        }
        offset += i.second.size();
        ofstream.rdbuf()->sputn(i.second.data(), i.second.size());
    }
}

bool FileRepository::putObject(const Object &obj) {
    // Put the object into index, and then write it to the storage
    {
        std::lock_guard lock(repoLock);
        keyIndex[obj.type][obj.getKey()] = obj.id;
    }
    writeObject(obj);
    return true;
}

bool FileRepository::deleteObject(const Object &obj) {
    if (!ready) throw Exception("Tried working with uninitialized repo!");
    throw Exception("Deletion not implemented!");
}

std::vector<char> FileRepository::readFile(const std::filesystem::path &file, unsigned long long offset,
                                           unsigned long long size) const {
    if (size > absoluteMaxFileLimit)
        throw Exception("Tried to read " + std::to_string(size) + " bytes from " + file.string() +
                        " which is more than absoluteMaxFileLimit");

    std::ifstream ifstream(file, std::ios::binary | std::ios::in);
    if (!ifstream.is_open()) throw Exception("Can't open file " + file.string() + " for reading!");

    std::vector<char> buf(size);

    if (ifstream.rdbuf()->pubseekpos(offset) == std::streampos(std::streamoff(-1)))
        throw Exception("Unexpected end of file " + file.string());
    if (ifstream.rdbuf()->sgetn(buf.data(), size) != size) throw Exception("Unexpected end of file " + file.string());

    return buf;
}

std::vector<char> FileRepository::readFile(const std::filesystem::path &file) const {
    if (!std::filesystem::is_regular_file(file)) throw Exception("File " + file.string() + " is not a regular file!");
    auto fileSize = std::filesystem::file_size(file);
    if (fileSize == 0) return {};
    return readFile(file, 0, fileSize);
}

bool FileRepository::writeFile(const std::filesystem::path &file, const std::vector<char> &data) {
    std::ofstream ofstream(file, std::ios::binary | std::ios::trunc | std::ios::out);
    if (!ofstream.is_open()) throw Exception("Can't open file " + file.string() + " for writing!");

    if (ofstream.rdbuf()->sputn(data.data(), data.size()) != data.size())
        throw Exception("Couldn't write all the data for " + file.string());
    return true;
}

std::vector<char> FileRepository::getObjectRaw(Object::ObjectType type, const std::string &key) const {
    return getObjectRaw(getObjectId(type, key));
}

Object::idType FileRepository::getObjectId(Object::ObjectType type, const std::string &key) const {
    std::lock_guard lock(repoLock);
    if (keyIndex.count(type) == 0) throw Exception("No objects of requested type!");
    return keyIndex.at(type).at(key);
}

std::vector<std::pair<std::string, Object::idType>> FileRepository::getObjects(Object::ObjectType type) const {
    std::lock_guard lock(repoLock);
    std::vector<std::pair<std::string, Object::idType>> out;
    if (keyIndex.count(type) == 0) return {};
    for (auto const &i: keyIndex.at(type)) out.emplace_back(i);
    return out;
}

bool FileRepository::exists(Object::ObjectType type, const std::string &key) const {
    std::lock_guard lock(repoLock);
    if (keyIndex.count(type) == 0) return false;
    return keyIndex.at(type).count(key) > 0;
}

Object::idType FileRepository::getId() {
    std::lock_guard lock(repoLock);
    return largestUnusedId++;
}

FileRepository::OffsetEntry::OffsetEntry(std::vector<char, std::allocator<char>>::const_iterator &in,
                                         const std::vector<char, std::allocator<char>>::const_iterator &end)
    : fileId(Serialize::deserialize<decltype(fileId)>(in, end)),
      offset(Serialize::deserialize<decltype(offset)>(in, end)),
      length(Serialize::deserialize<decltype(length)>(in, end)) {}

void FileRepository::OffsetEntry::serialize(std::vector<char> &out) const {
    Serialize::serialize(fileId, out);
    Serialize::serialize(offset, out);
    Serialize::serialize(length, out);
}

FileRepository::OffsetEntry::OffsetEntry(unsigned long long int fileId, unsigned long long int offset,
                                         unsigned long long int length)
    : fileId(fileId), offset(offset), length(length) {}
