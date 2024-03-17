//
// Created by Stepan Usatiuk on 14.04.2023.
//

#ifndef SEMBACKUP_OBJECT_H
#define SEMBACKUP_OBJECT_H

#include <cstdint>
#include <string>
#include <vector>

/// Base class for objects in the Repository
/**
 *  Every object has a unique id, and is also indexed by a type-key pair in the Repository cache
 */
class Object {
public:
    using idType = uint64_t;///< Type alias for Object%'s ID

    enum class ObjectType { Archive, File, Chunk, END };

    /// Serializes the object to \p out
    virtual void serialize(std::vector<char> &out) const;
    /// Signals the Serialization template to use Object's serialization/deserialization facilities
    using serializable = std::true_type;

    /// Default virtual destructor, don't allow to create an instance of Object
    virtual ~Object() = 0;

    /// Pure virtual function that returns the key by which will be the object indexed in the Repository cache
    /// All derived objects should implement this method
    virtual std::string getKey() const = 0;

    /// Returns the keys of that this object refers to
    virtual const std::vector<idType> &getRefs() const;

    const idType id;      ///< Unique numerical of the object
    const ObjectType type;///< Type of the object

protected:
    /// Default constructor
    /// \param id   Object ID
    /// \param type Object type
    Object(idType id, ObjectType type);

    /// Deserialization constructor
    Object(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end);
};


#endif//SEMBACKUP_OBJECT_H
