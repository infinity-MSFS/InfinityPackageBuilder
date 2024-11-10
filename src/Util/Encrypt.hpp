#pragma once

#include <cstring>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef WIN32
#include <Windows.h>

inline bool HideFile(const std::string &filePath) {
    if (SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_HIDDEN) == 0) {
        std::cerr << "Error: Could not hide the file. Error code: " << GetLastError() << std::endl;
        return false;
    }
    return true;
}
#endif

inline std::string BytesToHex(const std::vector<unsigned char> &bytes) {
    std::ostringstream oss;
    for (unsigned char byte: bytes) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return oss.str();
}

inline std::vector<unsigned char> HexToBytes(const std::string &hex) {
    std::vector<unsigned char> bytes;
    for (std::size_t i = 0; i < hex.length(); i += 2) {
        unsigned byte;
        std::istringstream(hex.substr(i, 2)) >> std::hex >> byte;
        bytes.push_back(static_cast<unsigned char>(byte));
    }
    return bytes;
}

inline std::string Encrypt(const std::string &plain_text, const std::string &key) {

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    unsigned char iv[EVP_MAX_IV_LENGTH];
    if (!RAND_bytes(iv, EVP_MAX_IV_LENGTH)) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to generate random IV");
    }

    std::vector<unsigned char> cipher_text(plain_text.size() + EVP_MAX_BLOCK_LENGTH);
    int len = 0, cipher_text_len = 0;

    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize encryption");
    }

    if (EVP_EncryptUpdate(ctx, cipher_text.data(), &len, reinterpret_cast<const unsigned char *>(plain_text.data()), static_cast<int>(plain_text.size())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to encrypt data");
    }
    cipher_text_len = len;

    if (EVP_EncryptFinal_ex(ctx, cipher_text.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize encryption");
    }
    cipher_text_len += len;
    cipher_text.resize(cipher_text_len);

    EVP_CIPHER_CTX_free(ctx);

    std::vector<unsigned char> iv_cipher_text(iv, iv + EVP_MAX_IV_LENGTH);
    iv_cipher_text.insert(iv_cipher_text.end(), cipher_text.begin(), cipher_text.end());

    return BytesToHex(iv_cipher_text);
}

inline std::string Decrypt(const std::string &hex_cipher_text, const std::string &key) {

    std::vector<unsigned char> iv_cipher_text = HexToBytes(hex_cipher_text);
    if (iv_cipher_text.size() < EVP_MAX_IV_LENGTH) {
        throw std::runtime_error("Invalid IV length in cipher text");
    }

    unsigned char iv[EVP_MAX_IV_LENGTH];
    std::memcpy(iv, iv_cipher_text.data(), EVP_MAX_IV_LENGTH);

    std::vector<unsigned char> cipher_text(iv_cipher_text.begin() + EVP_MAX_IV_LENGTH, iv_cipher_text.end());
    std::vector<unsigned char> plain_text(cipher_text.size() + EVP_MAX_BLOCK_LENGTH);

    int len = 0, plain_text_len = 0;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
    }

    if (key.size() != 32) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Key must be 32 bytes for AES-256");
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to initialize decryption");
    }

    if (EVP_DecryptUpdate(ctx, plain_text.data(), &len, cipher_text.data(), static_cast<int>(cipher_text.size())) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to decrypt data during update");
    }
    plain_text_len = len;

    if (EVP_DecryptFinal_ex(ctx, plain_text.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Failed to finalize decryption");
    }
    plain_text_len += len;

    plain_text.resize(plain_text_len);
    EVP_CIPHER_CTX_free(ctx);


    return {plain_text.begin(), plain_text.end()};
}
