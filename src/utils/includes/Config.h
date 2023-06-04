//
// Created by Stepan Usatiuk on 01.05.2023.
//

#ifndef SEMBACKUP_CONFIG_H
#define SEMBACKUP_CONFIG_H

#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

/// Utility class to manage configuration
/**
 *  Also provides keys map for information about config keys
 *  Serializable, remembers only the keys with remember option set in keys
 */
class Config {
public:
    /// Constructs an empty Config instance
    Config();
    /// Deserialization constructor
    Config(std::vector<char>::const_iterator &in, const std::vector<char>::const_iterator &end);

    /// Adds a key \p k with value \p v to the config
    /// \param k Const reference to the config key
    /// \param v Config value
    /// \return  Reference to itself
    /// \throws  Exception if key is invalid or is already set with different value
    Config &add(const std::string &k, const std::string &v);

    /// Merges \p config to itself
    /// Adds every config pair from \p config to itself, throws on conflict
    /// \param config    Constant reference to the source Config
    /// \throws          Exception on merge conflict
    void merge(const Config &config);

    /// Returns an int from config key \p k
    /// \param k Constant reference to the key string
    /// \return  Config int
    /// \throws  Exception if key is invalid or value isn't an int
    int getInt(const std::string &k) const;

    /// Returns a string from config key \p k
    /// \param k Constant reference to the key string
    /// \return  Config value for key
    /// \throws  Exception if key is invalid
    std::string getStr(const std::string &k) const;

    /// Returns a list of strings delimited by commas from config key \p k
    /// \param k Constant reference to the key string
    /// \return  Vector of strings
    /// \throws  Exception if key is invalid
    std::vector<std::string> getList(const std::string &k) const;

    /// Checks if key \p k exists in the config
    /// \param k Constant reference to the key string
    /// \return  True if key exists or its default value exists
    bool exists(const std::string &k) const;

    /// Serialization function
    void serialize(std::vector<char> &out) const;

    using serializable = std::true_type;

    enum class KeyType {
        STRING,
        INT,
        LIST
    };

    /// Struct to record key options
    struct keyopts {
        std::optional<std::string> defaultval;///< Key's default value
        KeyType type;                         ///< Key's type
        bool remember;                        ///< Whether the key should be serialized
        std::string info;                     ///< Printed in help
    };

    /// Used for printing help
    const static inline std::unordered_map<KeyType, std::string> KeyTypeToStr{{KeyType::STRING, "string"}, {KeyType::INT, "number"}, {KeyType::LIST, "comma-separated list"}};

    /// Default values and their metadata
    const static inline std::unordered_map<std::string, keyopts> keys{
            {"compression", {"none", KeyType::STRING, true, "Compression algorighm to use (zlib or none)"}},
            {"encryption", {"none", KeyType::STRING, true, "Encryption algorighm to use (aes or none)"}},
            {"compression-level", {"-1", KeyType::INT, true, "Compression level to use (0 to 9)"}},
            {"repo", {std::nullopt, KeyType::STRING, false, "Repository root"}},
            {"to", {std::nullopt, KeyType::STRING, false, "Destination of restore"}},
            {"from", {std::nullopt, KeyType::STRING, true, "Backed up folder"}},
            {"type", {"normal", KeyType::STRING, false, "Type of archive"}},
            {"aid", {std::nullopt, KeyType::INT, false, "ID of archive to restore/compare to"}},
            {"aid2", {std::nullopt, KeyType::INT, false, "ID of archive to compare with"}},
            {"threads", {std::nullopt, KeyType::INT, false, "Number of threads to use"}},
            {"prefix", {"", KeyType::STRING, false, "Prefix of files to compare"}},
            {"password", {std::nullopt, KeyType::STRING, false, "Encryption password"}},
            {"salt", {std::nullopt, KeyType::STRING, true, "Encryption salt"}},
            {"chunker", {"buzhash", KeyType::STRING, true, "Chunker to use (const, buzhash)"}},
            {"chunker-min", {"256", KeyType::INT, true, "Min chunk size in KB"}},
            {"chunker-max", {"4096", KeyType::INT, true, "Max chunk size in KB"}},
            {"chunker-mask", {"20", KeyType::INT, true, "Chunker hash bit mask (mask of n bits results in average chunk size of 2^n bytes)"}},
            {"repo-target", {"128", KeyType::INT, true, "Target size of files for FileRepository"}},
            {"full-period", {"2", KeyType::INT, true, "Interval between forced full backups"}},
            {"progress", {"pretty", KeyType::STRING, false, "How to print progress (simple, pretty, none)"}},
            {"verbose", {"1", KeyType::INT, false, "Message verbosity (0 - error, 1 - info, -1 - quiet)"}},
            {"dedup", {"on", KeyType::STRING, true, "Turns deduplication on/off"}},
            {"change-detectors", {"type,size,etime", KeyType::LIST, true, "Change detectors to use (in order)"}},
            {"diff-mode", {"normal", KeyType::STRING, false, "Diff mode (file or normal)"}},
    };

private:
    std::unordered_map<std::string, std::string> data;
};


#endif//SEMBACKUP_CONFIG_H
