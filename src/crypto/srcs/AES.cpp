//
// Created by Stepan Usatiuk on 30.04.2023.
//

#include "AES.h"

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include "Exception.h"

std::vector<char> AES::encrypt(const std::vector<char> &in, const std::string &password, const std::string &salt) {
    return AES::encrypt(in, AES::deriveKey(password, salt));
}

std::vector<char> AES::decrypt(const std::vector<char> &in, const std::string &password, const std::string &salt) {
    return AES::decrypt(in, AES::deriveKey(password, salt));
}

std::vector<char> AES::encrypt(const std::vector<char> &in, const std::array<uint8_t, 32> &key) {
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
    if (!ctx) throw Exception("Error initializing encryption context!");

    std::vector<char> out(in.size() + AES_BLOCK_SIZE + 32);
    if (!RAND_bytes(reinterpret_cast<unsigned char *>(out.data()), 32)) throw Exception("Error generating IV!");

    if (!EVP_EncryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key.data(),
                            reinterpret_cast<const unsigned char *>(out.data())))
        throw Exception("Error encrypting!");

    int outlen = static_cast<int>(out.size()) - 32;

    if (!EVP_EncryptUpdate(ctx.get(), reinterpret_cast<unsigned char *>(out.data() + 32), &outlen,
                           reinterpret_cast<const unsigned char *>(in.data()), static_cast<int>(in.size())))
        throw Exception("Error encrypting!");

    int finlen = 0;
    if (!EVP_EncryptFinal_ex(ctx.get(), reinterpret_cast<unsigned char *>(out.data() + outlen + 32), &finlen))
        throw Exception("Error encrypting!");

    out.resize(outlen + finlen + 32);

    return out;
}

std::vector<char> AES::decrypt(const std::vector<char> &in, const std::array<uint8_t, 32> &key) {
    if (in.size() < 32) throw Exception("Array to decrypt is too small!");

    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)> ctx(EVP_CIPHER_CTX_new(), &EVP_CIPHER_CTX_free);
    if (!ctx) throw Exception("Error initializing encryption context!");

    std::vector<char> out(in.size() - 32);
    int outlen = static_cast<int>(out.size());

    if (!EVP_DecryptInit_ex(ctx.get(), EVP_aes_256_cbc(), nullptr, key.data(),
                            reinterpret_cast<const unsigned char *>(in.data())))
        throw Exception("Error decrypting!");


    if (!EVP_DecryptUpdate(ctx.get(), reinterpret_cast<unsigned char *>(out.data()), &outlen,
                           reinterpret_cast<const unsigned char *>(in.data() + 32), static_cast<int>(in.size() - 32)))
        throw Exception("Error decrypting!");

    int finlen = 0;
    if (!EVP_DecryptFinal_ex(ctx.get(), (unsigned char *) (out.data() + outlen), &finlen))
        throw Exception("Error decrypting!");

    out.resize(outlen + finlen);

    return out;
}

std::array<uint8_t, 32> AES::deriveKey(const std::string &password, const std::string &salt) {
    std::array<uint8_t, 32> key;//NOLINT
    if (!PKCS5_PBKDF2_HMAC_SHA1(password.data(), static_cast<int>(password.length()),
                                reinterpret_cast<const unsigned char *>(salt.data()), static_cast<int>(salt.length()),
                                10000, 32, key.data()))
        throw Exception("Error deriving key!");
    return key;
}
