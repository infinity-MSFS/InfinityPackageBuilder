
#include "Encryption.hpp"

#include <algorithm>

namespace Encryption {
    std::vector<unsigned char> Encryption::GenerateIV() {
        std::vector<unsigned char> IV(EVP_MAX_IV_LENGTH);
        if (!RAND_bytes(IV.data(), EVP_MAX_IV_LENGTH)) {
            throw std::runtime_error("Failed to generate IV");
        }
        return IV;
    }

    std::vector<unsigned char> Encryption::Encrypt(const std::string &plaintext, const std::string &key) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }
        const auto IV = GenerateIV();

        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), IV.data())) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize encryption");
        }

        std::vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH + EVP_MAX_IV_LENGTH);

        std::ranges::copy(IV, ciphertext.begin());
        int offset = EVP_MAX_IV_LENGTH;

        int len;
        if (!EVP_EncryptUpdate(ctx, ciphertext.data() + offset, &len, reinterpret_cast<const unsigned char *>(plaintext.c_str()), static_cast<int>(plaintext.size()))) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to encrypt data");
        }
        offset += len;

        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + offset, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize encryption");
        }
        offset += len;

        EVP_CIPHER_CTX_free(ctx);

        ciphertext.resize(offset);
        return ciphertext;
    }

    std::string Encryption::Decrypt(const std::vector<unsigned char> &ciphertext, const std::string &key) {
        if (ciphertext.size() <= EVP_MAX_IV_LENGTH) {
            throw std::runtime_error("Ciphertext too short");
        }

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }

        const std::vector iv(ciphertext.begin(), ciphertext.begin() + EVP_MAX_IV_LENGTH);

        if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize decryption");
        }

        std::vector<unsigned char> plaintext(ciphertext.size() - EVP_MAX_IV_LENGTH);
        int len;

        if (!EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data() + EVP_MAX_IV_LENGTH, static_cast<int>(ciphertext.size()) - EVP_MAX_IV_LENGTH)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to decrypt data");
        }
        int plaintext_len = len;

        if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize decryption");
        }
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        return {plaintext.begin(), plaintext.begin() + plaintext_len};
    }

    std::vector<uint8_t> Encryption::EncryptBinary(const std::vector<uint8_t> &binary_input, const std::string &key) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }

        std::vector<unsigned char> iv = GenerateIV();

        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize encryption");
        }

        std::vector<uint8_t> ciphertext(binary_input.size() + EVP_MAX_BLOCK_LENGTH + EVP_MAX_IV_LENGTH);

        std::ranges::copy(iv, ciphertext.begin());
        int offset = EVP_MAX_IV_LENGTH;

        int len;
        if (!EVP_EncryptUpdate(ctx, ciphertext.data() + offset, &len, binary_input.data(), static_cast<int>(binary_input.size()))) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to encrypt data");
        }
        offset += len;

        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + offset, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize encryption");
        }
        offset += len;

        EVP_CIPHER_CTX_free(ctx);

        ciphertext.resize(offset);
        return ciphertext;
    }

    std::vector<uint8_t> Encryption::DecryptBinary(const std::vector<uint8_t> &ciphertext, const std::string &key) {
        if (ciphertext.size() <= EVP_MAX_IV_LENGTH) {
            throw std::runtime_error("Ciphertext too short");
        }

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }

        const std::vector iv(ciphertext.begin(), ciphertext.begin() + EVP_MAX_IV_LENGTH);

        if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, reinterpret_cast<const unsigned char *>(key.c_str()), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize decryption");
        }

        std::vector<uint8_t> plaintext(ciphertext.size() - EVP_MAX_IV_LENGTH);
        int len;

        if (!EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data() + EVP_MAX_IV_LENGTH, static_cast<int>(ciphertext.size() - EVP_MAX_IV_LENGTH))) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to decrypt data");
        }
        int plaintext_len = len;

        if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize decryption");
        }
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        plaintext.resize(plaintext_len);
        return plaintext;
    }


    std::vector<unsigned char> Encryption::Base64Decode(const std::string &encoded_data) {
        std::vector<unsigned char> result(encoded_data.size());

        BIO *b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO *bio = BIO_new_mem_buf(encoded_data.data(), -1);
        bio = BIO_push(b64, bio);

        const int decoded_size = BIO_read(bio, result.data(), static_cast<int>(encoded_data.size()));
        BIO_free_all(bio);

        if (decoded_size < 0) {
            throw std::runtime_error("Failed to decode base64 data");
        }

        result.resize(decoded_size);
        return result;
    }

    std::string Encryption::Base64Encode(const std::vector<unsigned char> &binary_data) {
        BUF_MEM *bufferPtr;

        BIO *b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO *bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_write(bio, binary_data.data(), static_cast<int>(binary_data.size()));
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &bufferPtr);

        std::string result(bufferPtr->data, bufferPtr->length);

        BIO_free_all(bio);

        return result;
    }


    std::string Encryption::EncryptToBase64(const std::string &plaintext, const std::string &key) {
        const auto encrypted = Encrypt(plaintext, key);
        return Base64Encode(encrypted);
    }

    std::string Encryption::DecryptFromBase64(const std::string &base64_data, const std::string &key) {
        const auto binary_data = Base64Decode(base64_data);
        return Decrypt(binary_data, key);
    }


    Validation::Validation(const char *pub_key) : m_PublicKey(pub_key), m_PrivateKey(), m_ValidationType(ValidationTypes::NOT_AUTHORIZED) {}

    void Validation::ParsePublicKey() { m_PrivateKey = Encryption::DecryptFromBase64(m_PublicKey, keys::group_decrypt_key).c_str(); }

    ValidationTypes Validation::GetValidationType() {
        ParsePublicKey();
        if (strcmp(m_PrivateKey, keys::admin_key) == 0) {
            m_ValidationType = ValidationTypes::FULL;
        } else if (strcmp(m_PrivateKey, keys::aero_dynamics_key) == 0) {
            m_ValidationType = ValidationTypes::AERO_DYNAMICS;
        } else if (strcmp(m_PrivateKey, keys::delta_sim_key) == 0) {
            m_ValidationType = ValidationTypes::DELTA_SIM;
        } else if (strcmp(m_PrivateKey, keys::lunar_sim_key) == 0) {
            m_ValidationType = ValidationTypes::LUNAR_SIM;
        } else if (strcmp(m_PrivateKey, keys::ouroboros_key) == 0) {
            m_ValidationType = ValidationTypes::OUROBOROS;
        } else if (strcmp(m_PrivateKey, keys::qbit_sim_key) == 0) {
            m_ValidationType = ValidationTypes::QBIT_SIM;
        } else {
            m_ValidationType = ValidationTypes::NOT_AUTHORIZED;
        }
        return m_ValidationType;
    }
} // namespace Encryption
