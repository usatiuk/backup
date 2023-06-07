//
// Created by Stepan Usatiuk on 15.04.2023.
//

#ifndef SEMBACKUP_CHUNKER_H
#define SEMBACKUP_CHUNKER_H

#include <array>
#include <optional>
#include <streambuf>
#include <vector>

/// Abstract base class for a Chunker that takes a file and splits it into chunks to be backed up
class Chunker {
private:
    /// Convenience iterator to allow using Chunker%s in range for loops
    struct ChunkerIterator {
        using value_type = std::pair<std::string, std::vector<char>>;

        /// Creates a ChunkerIterator pointing to the first chunk or past-EOF
        /// \param source Pointer to a Chunker, should be available during the entire iterator lifetime, or nullptr if this is pastEOF iterator
        ChunkerIterator(Chunker *source);

        /// Increments the iterator to the next chunk, or past-EOF
        /// \throws Exception if iterator points past-EOF
        ChunkerIterator &operator++();

        /// Returns the current pointed-to chunk
        /// \throws Exception if iterator points past-EOF
        value_type operator*() const;

        /// Returns true if both iterators are past-EOF
        bool operator==(const ChunkerIterator &rhs) const;

        /// Returns false if both iterators are past-EOF
        bool operator!=(const ChunkerIterator &rhs) const;

    private:
        Chunker *const source;        ///< Pointer to the underlying Chunker
        std::optional<value_type> buf;///< Currently pointed to chunk
        bool pastEOF = false;         ///< Whether past EOF has been reached
    };

public:
    /// Returns the next chunk of the file
    /// Returns a single empty chunk if a file is empty
    /// \return Pair consisting of chunk's bytes and its SHA hash
    /// \throws Exception if EOF was already reached
    virtual std::pair<std::string, std::vector<char>> getNext() = 0;

    /// Returns True if EOF was reached, False otherwise
    bool getEof() const;

    /// Default virtual destructor
    virtual ~Chunker();

    /// Returns a ChunkerIterator pointing to the first chunk in a file
    ChunkerIterator begin();

    /// Returns a past-EOF ChunkerIterator
    static ChunkerIterator end();

protected:
    /// \param buf          Pointer to a std::streambuf, should be available during the entire lifetime of a Chunker
    /// \param maxBytes     Maximal amount of bytes in returned chunks
    Chunker(std::streambuf *buf, unsigned long long maxBytes);

    std::streambuf *const buf;        ///< Constant pointer to the source std::streambuf
    bool eof = false;                 ///< Indicates whether EOF has been reached
    const unsigned long long maxBytes;///< Max number of bytes in returned chunks
};


#endif//SEMBACKUP_CHUNKER_H
