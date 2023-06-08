//
// Created by Stepan Usatiuk on 14.04.2023.
//

#include "../../includes/objects/File.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../../../utils/includes/Exception.h"
#include "../../includes/Serialize.h"

File::File(Object::idType id, std::string name, unsigned long long bytes, unsigned long long mtime, std::string SHA, std::map<size_t, idType> chunks, Type fileType)
    : Object(id, ObjectType::File), name(name), bytes(bytes), mtime(mtime), SHA(SHA), fileType(fileType), chunks(std::move(chunks)) {}

File::File(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end)
    : Object(in, end),
      name(Serialize::deserialize<std::string>(in, end)),
      bytes(Serialize::deserialize<unsigned long long>(in, end)),
      mtime(Serialize::deserialize<unsigned long long>(in, end)),
      SHA(Serialize::deserialize<std::remove_const<decltype(SHA)>::type>(in, end)),
      fileType(Serialize::deserialize<std::remove_const<decltype(fileType)>::type>(in, end)),
      chunks(Serialize::deserialize<std::remove_const<decltype(chunks)>::type>(in, end)) {
    if (type != ObjectType::File) throw Exception("Type mismatch for File!");
}

void File::serialize(std::vector<char> &out) const {
    Object::serialize(out);
    Serialize::serialize(name, out);
    Serialize::serialize(bytes, out);
    Serialize::serialize(mtime, out);
    Serialize::serialize(SHA, out);
    Serialize::serialize(fileType, out);
    Serialize::serialize(chunks, out);
}

std::string File::getKey() const {
    return name;
}

File::Type File::getFileType(const std::filesystem::path &p) {
    if (std::filesystem::is_symlink(p)) return Type::Symlink;
    if (std::filesystem::is_directory(p)) return Type::Directory;
    if (std::filesystem::is_regular_file(p)) return Type::Normal;
    throw Exception("Unsupported file type! " + p.u8string());
}

std::vector<char> File::getFileContents(const std::filesystem::path &p) {
    auto type = getFileType(p);
    if (type == Type::Normal) throw Exception(p.u8string() + " is a normal file!");
    if (type == Type::Directory) {
        return {};
    }
    if (type == Type::Symlink) {
        auto target = std::filesystem::read_symlink(p).u8string();
        return {target.begin(), target.end()};
    }
    throw Exception("Error with file " + p.u8string());
}

unsigned long long File::getFileMtime(const std::filesystem::path &p) {
    auto type = getFileType(p);
    if (type == Type::Normal || type == Type::Directory)
        return static_cast<const unsigned long long int>(std::chrono::duration_cast<std::chrono::seconds>(std::filesystem::last_write_time(p).time_since_epoch()).count());
    else if (type == Type::Symlink) {
        auto path = p.u8string();
        struct stat sb;
        if (lstat(path.c_str(), &sb) != 0) throw Exception("Error reading mtime for " + p.u8string());
#ifdef __APPLE__
        return sb.st_mtimespec.tv_sec;
#else
        return sb.st_mtime;
#endif
    }
    throw Exception("Error with file " + p.u8string());
}

unsigned long long File::getFileSize(const std::filesystem::path &p) {
    auto type = getFileType(p);
    if (type == Type::Normal) return std::filesystem::file_size(p);
    else
        return getFileContents(p).size();
}
