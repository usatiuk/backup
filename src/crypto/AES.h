//
// Created by Stepan Usatiuk on 30.04.2023.
//

#ifndef SEMBACKUP_AES_H
#define SEMBACKUP_AES_H

#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

/// Utility class to handle encryption/decryption of byte vectors
/**
 *  Based on: https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
 */
class AES {
public:
    /// Encrypts the provided \p in vector using \p password and \p salt
    /// \param in           Constant reference to to-be-encrypted vector
    /// \param password     Constant reference to the password
    /// \param salt         Constant reference to the salt
    /// \return             Encrypted vector of size at most original + 48 (16 for possible padding, 32 for the IV)
    /// \throws             Exception on any error
    static std::vector<char> encrypt(const std::vector<char> &in, const std::string &password, const std::string &salt);

    /// Decrypts the provided \p in vector using \p password and \p salt
    /// \param in           Constant reference to to-be-decrypted vector
    /// \param password     Constant reference to the password
    /// \param salt         Constant reference to the salt
    /// \return             Decrypted vector
    /// \throws             Exception on any error
    static std::vector<char> decrypt(const std::vector<char> &in, const std::string &password, const std::string &salt);

    /// Encrypts the provided \p in vector using \p key
    /// \param in           Constant reference to to-be-encrypted vector
    /// \param key          Constant reference to the key
    /// \return             Encrypted vector of size at most original + 48 (16 for possible padding, 32 for the IV)
    /// \throws             Exception on any error
    static std::vector<char> encrypt(const std::vector<char> &in, const std::array<uint8_t, 32> &key);

    /// Decrypts the provided \p in vector using \p key
    /// \param in           Constant reference to to-be-decrypted vector
    /// \param key          Constant reference to the key
    /// \return             Decrypted vector
    /// \throws             Exception on any error
    static std::vector<char> decrypt(const std::vector<char> &in, const std::array<uint8_t, 32> &key);

    /// Generates a key for the encryption using \p password and \p salt using PKCS5_PBKDF2_HMAC_SHA1
    /// \param password     Constant reference to the password
    /// \param salt         Constant reference to the salt
    /// \return             Derived key
    /// \throws             Exception on any error
    static std::array<uint8_t, 32> deriveKey(const std::string &password, const std::string &salt);
};


#endif//SEMBACKUP_AES_H
