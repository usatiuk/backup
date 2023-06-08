//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_FILE_H
#define SEMBACKUP_FILE_H

#include <array>
#include <filesystem>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Object.h"

/// Object representing a saved file
class File : public Object {
public:
    enum class Type {
        Normal,
        Symlink,
        Directory,
        END
    };

    static inline const std::unordered_map<Type, std::string> TypeToStr{{Type::Normal, "normal"}, {Type::Symlink, "symlink"}, {Type::Directory, "directory"}};

    File(Object::idType id, std::string name, unsigned long long bytes, unsigned long long mtime, std::string SHA, std::map<size_t, idType> chunks, Type fileType);

    /// Deserialization constructor
    File(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end);

    /// \copydoc Object::serialize
    void serialize(std::vector<char> &out) const override;

    /// Returns the file relative path as key
    std::string getKey() const override;

    /// Helper static function to return Type for any file in the filesystem
    /// \param p Constant reference to the absolute path of the file
    /// \return  Type of the file
    static Type getFileType(const std::filesystem::path &p);

    /// Helper static function to return "contents" for non-regular files in the filesystem
    /// \param p Constant reference to the absolute path of the file
    /// \return  File contents, (for symlinks - its destination, for directory - empty)
    /// \throws  Exception on any error, or if file is regular
    static std::vector<char> getFileContents(const std::filesystem::path &p);

    /// Helper static function to return modification time for files in the filesystem
    /// \param p Constant reference to the absolute path of the file
    /// \return  File last modification time
    /// \throws  Exception on any error
    static unsigned long long getFileMtime(const std::filesystem::path &p);

    /// Helper static function to return file size for files in the filesystem
    /// \param p Constant reference to the absolute path of the file
    /// \return  File size
    /// \throws  Exception on any error
    static unsigned long long getFileSize(const std::filesystem::path &p);


    const std::string name;        ///< Relative path to backup root, as UTF-8 string
    const unsigned long long bytes;///< Amount of bytes in the file
    const unsigned long long mtime;///< Last modification time as timestamp
    const std::string SHA;         ///< Hash of the file
    const Type fileType;           ///< File type

    /// List of the chunks in file
    /// Normal file has normal chunks as its contents, for Directory it's empty, Symlink has a chunk with its target path
    const std::map<size_t, idType> chunks;
};


#endif//SEMBACKUP_FILE_H
