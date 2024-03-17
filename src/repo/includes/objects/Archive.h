//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_ARCHIVE_H
#define SEMBACKUP_ARCHIVE_H

#include <array>

#include "../Object.h"

/// Object representing a backup
class Archive final : public Object {
public:
    Archive(Object::idType id, std::string name, unsigned long long mtime, std::vector<idType> files);

    /// \copydoc Object::serialize
    Archive(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end);

    void serialize(std::vector<char> &out) const override;

    /// Returns the name of the archive
    std::string getKey() const override;

    /// Returns the files in this archive
    const std::vector<Object::idType> &getRefs() const override;

    const std::string name;         ///< Archive name
    const unsigned long long mtime; ///< Time of creation
    const std::vector<idType> files;///< List of ids of File objects in the Archive
};


#endif//SEMBACKUP_ARCHIVE_H
