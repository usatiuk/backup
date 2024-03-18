//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_REPOSITORY_H
#define SEMBACKUP_REPOSITORY_H

#include <filesystem>
#include <mutex>
#include <set>

#include "Config.h"
#include "FilterContainer.h"
#include "Object.h"

/// Abstract base class for an Object Repository
/**
 *  So far only FileRepository exists, and probably this interface is too inflexible
 *  to be easily used for creating other (database, object storage...) repositories,
 *  but it should be possible with some refactoring
 */

class Repository {
public:
    /// Checks if a repository already exists
    /// \return True if exists, False otherwise
    virtual bool exists() = 0;

    /// Tries to open the Repository
    /// \return True
    /// \throws Exception on any error
    virtual bool open() = 0;

    /// Tries to initialize the Repository
    /// \return True
    /// \throws Exception on any error, including if the Repository is already initialized
    virtual bool init() = 0;

    /// Tries to flush the Repository write cache
    /// \returns True
    /// \throws  Exception on any error
    virtual bool flush() = 0;

    /// Returns the serialized Object with id \p id
    /// \param id ID of object to return
    /// \return   Serialized object
    /// \throws   Exception on any error or if object doesn't exist
    virtual std::vector<char> getObjectRaw(Object::idType id) const = 0;

    /// Returns the Object with id \p id
    /// \param id ID of object to return
    /// \return   Serialized object
    /// \throws   Exception on any error or if object doesn't exist
    std::unique_ptr<Object> getObject(Object::idType id) const;

    /// Adds the Object \p obj to the Repository
    /// \param obj  Constant reference to the object
    /// \return     True
    /// \throws     Exception on any error
    virtual bool putObject(const Object &obj) = 0;

    /// Deletes Object \p obj from the Repository
    /// \param obj  Constant reference to the vector with ids of objects to delete
    /// \return     True if successful, False if it didn't exist
    /// \throws     Exception on any error
    virtual bool deleteObjects(const std::vector<Object::idType> &objs) = 0;

    /// Returns the Object of type \p type and with key \p key
    /// \param type Type of the object
    /// \param key  Constant reference to the key of the object
    /// \return     Serialized object
    /// \throws   Exception on any error or if object doesn't exist
    virtual std::vector<char> getObjectRaw(Object::ObjectType type, const std::string &key) const = 0;

    /// Returns the id of an Object of type \p type and with key \p key
    /// \param type Type of the object
    /// \param key  Constant reference to the key of the object
    /// \return     ID of the object
    /// \throws   Exception on any error or if object doesn't exist
    virtual Object::idType getObjectId(Object::ObjectType type, const std::string &key) const = 0;

    /// Returns the list of Objects of type \p type
    /// \param type Type of the object
    /// \return     Vector of pairs <key of object, id of object>
    /// \throws     Exception on any error
    virtual std::vector<std::pair<std::string, Object::idType>> getObjects(Object::ObjectType type) const = 0;

    /// Returns whether Object of type \p type and with key \p key exists
    /// \param type Type of the object
    /// \param key  Constant reference to the key of the object
    /// \return     True if exists, False otherwise
    /// \throws     Exception on any error
    virtual bool exists(Object::ObjectType type, const std::string &key) const = 0;

    /// Returns the next available object id
    virtual Object::idType getId() = 0;

    /// Returns the const reference to Config object used for this Repository
    const Config &getConfig() const;

    /// Default virtual destructor
    virtual ~Repository();

    Repository(const Repository &r) = delete;
    Repository &operator=(const Repository &r) = delete;

protected:
    /// Base Repository class constructor
    /// \param config   Config to use
    Repository(Config config);

    Config config;          ///< Config of this Repository
    FilterContainer filters;///< Container of IO filters used to transform Objects when writing/reading to/from storage
};


#endif//SEMBACKUP_REPOSITORY_H
