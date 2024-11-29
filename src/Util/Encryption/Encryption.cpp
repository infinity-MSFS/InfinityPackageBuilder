
#include "Encryption.hpp"

#include <fstream>
#include <ranges>

namespace Infinity {
    ValidationTypes Key::GetValidationType(const Key &provided_key) {
        if (Encryption::DecryptToBin(provided_key.key, CreateUnencryptedKey(group_key)) == CreateKey(admin_key, CreateUnencryptedKey(group_key))) {
            return ValidationTypes::FULL;
        }
        if (Encryption::DecryptToBin(provided_key.key, CreateUnencryptedKey(group_key)) == CreateKey(aero_dynamics_key, CreateUnencryptedKey(group_key))) {
            return ValidationTypes::AERO_DYNAMICS;
        }
        if (Encryption::DecryptToBin(provided_key.key, CreateUnencryptedKey(group_key)) == CreateKey(delta_sim_key, CreateUnencryptedKey(group_key))) {
            return ValidationTypes::DELTA_SIM;
        }
        if (Encryption::DecryptToBin(provided_key.key, CreateUnencryptedKey(group_key)) == CreateKey(lunar_sim_key, CreateUnencryptedKey(group_key))) {
            return ValidationTypes::LUNAR_SIM;
        }
        if (Encryption::DecryptToBin(provided_key.key, CreateUnencryptedKey(group_key)) == CreateKey(ouroboros_key, CreateUnencryptedKey(group_key))) {
            return ValidationTypes::OUROBOROS;
        }
        if (Encryption::DecryptToBin(provided_key.key, CreateUnencryptedKey(group_key)) == CreateKey(qbit_sim_key, CreateUnencryptedKey(group_key))) {
            return ValidationTypes::QBIT_SIM;
        }
        return ValidationTypes::NOT_AUTHORIZED;
    }


    v_bytes Key::ParseKeyFile(const std::string &file_path) {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            Error(ErrorType::Warning, "Failed to open file: " + file_path).Dispatch();
        }

        const std::streamsize file_size = file.tellg();
        if (file_size < 0) {
            Error(ErrorType::Warning, "Failed to get file size: " + file_path);
        }

        v_bytes buffer(static_cast<size_t>(file_size));

        file.seekg(0, std::ios::beg);
        if (!file.read(reinterpret_cast<char *>(buffer.data()), file_size)) {
            Error(ErrorType::Warning, "Failed to read file" + file_path).Dispatch();
        }

        return buffer;
    }


    v_bytes Encryption::Encrypt(const std::string &plain_text, const v_bytes &key) {
        if (key.size() != 32) {
            Error(ErrorType::Fatal, "Attempted encryption with invalid key").Dispatch();
        }
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            Error(ErrorType::Fatal, "Failed to create encryption cipher context").Dispatch();
        }

        v_bytes iv = GenerateIV();

        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed to initialize encryption").Dispatch();
        }

        v_bytes ciphertext(plain_text.size() + EVP_MAX_BLOCK_LENGTH + EVP_MAX_IV_LENGTH);

        std::ranges::copy(iv, ciphertext.begin());
        int offset = EVP_MAX_IV_LENGTH;
        int len;
        if (!EVP_EncryptUpdate(ctx, ciphertext.data() + offset, &len, reinterpret_cast<const unsigned char *>(plain_text.c_str()), static_cast<int>(plain_text.size()))) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed encrypt data").Dispatch();
        }
        offset += len;

        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + offset, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed finalize encryption").Dispatch();
        }
        offset += len;

        EVP_CIPHER_CTX_free(ctx);

        ciphertext.resize(offset);
        return ciphertext;
    }

    v_bytes Encryption::Encrypt(const v_bytes &binary, const v_bytes &key) {
        if (key.size() != 32) {
            Error(ErrorType::Fatal, "Attempted encryption with invalid key").Dispatch();
        }
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            Error(ErrorType::Fatal, "Failed to create encryption cipher context").Dispatch();
        }

        v_bytes iv = GenerateIV();

        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed to initialize encryption").Dispatch();
        }

        v_bytes ciphertext(binary.size() + EVP_MAX_BLOCK_LENGTH + EVP_MAX_IV_LENGTH);

        std::ranges::copy(iv, ciphertext.begin());
        int offset = EVP_MAX_IV_LENGTH;

        int len;
        if (!EVP_EncryptUpdate(ctx, ciphertext.data() + offset, &len, binary.data(), static_cast<int>(binary.size()))) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed encrypt data").Dispatch();
        }

        offset += len;

        if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + offset, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed finalize encryption").Dispatch();
        }
        offset += len;

        EVP_CIPHER_CTX_free(ctx);

        ciphertext.resize(offset);
        return ciphertext;
    }

    std::string Encryption::EncryptBase64(const std::string &plain_text, const v_bytes &key) {
        const auto bytes = Encrypt(plain_text, key);
        return Base64Encode(bytes);
    }
    std::string Encryption::EncryptBase64(const v_bytes &binary, const v_bytes &key) {
        const auto bytes = Encrypt(binary, key);
        return Base64Encode(bytes);
    }

    std::string Encryption::Decrypt(const v_bytes &binary, const v_bytes &key) {
        if (key.size() != 32) {
            Error(ErrorType::Fatal, "Attempted decryption with invalid key").Dispatch();
        }

        if (binary.size() <= EVP_MAX_IV_LENGTH) {
            Error(ErrorType::Fatal, "Binary too short").Dispatch();
        }

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            Error(ErrorType::Fatal, "Failed to create cipher context").Dispatch();
        }

        const std::vector iv(binary.begin(), binary.begin() + EVP_MAX_IV_LENGTH);

        if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed to initialize decryption").Dispatch();
        }


        v_bytes plain_text(binary.size() - EVP_MAX_IV_LENGTH);
        int len;

        if (!EVP_DecryptUpdate(ctx, plain_text.data(), &len, binary.data() + EVP_MAX_IV_LENGTH, static_cast<int>(binary.size()) - EVP_MAX_IV_LENGTH)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to decrypt data");
        }
        int plaintext_len = len;

        if (!EVP_DecryptFinal_ex(ctx, plain_text.data() + len, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to finalize decryption");
        }
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        return {plain_text.begin(), plain_text.begin() + plaintext_len};
    }


    v_bytes Encryption::DecryptToBin(const v_bytes &binary, const v_bytes &key) {
        if (key.size() != 32) {
            Error(ErrorType::Fatal, "Attempted decryption with invalid key").Dispatch();
        }

        if (binary.size() <= EVP_MAX_IV_LENGTH) {
            Error(ErrorType::Fatal, "Binary too short").Dispatch();
        }

        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            Error(ErrorType::Fatal, "Failed to create cipher context").Dispatch();
        }

        const std::vector iv(binary.begin(), binary.begin() + EVP_MAX_IV_LENGTH);

        if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed to initialize decryption").Dispatch();
        }

        std::vector<uint8_t> plaintext(binary.size() - EVP_MAX_IV_LENGTH);
        int len;

        if (!EVP_DecryptUpdate(ctx, plaintext.data(), &len, binary.data() + EVP_MAX_IV_LENGTH, static_cast<int>(binary.size() - EVP_MAX_IV_LENGTH))) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed to decrypt data").Dispatch();
        }
        int plaintext_len = len;

        if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
            EVP_CIPHER_CTX_free(ctx);
            Error(ErrorType::Fatal, "Failed to finalize decryption").Dispatch();
        }
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        plaintext.resize(plaintext_len);
        return plaintext;
    }

    std::string Encryption::DecryptBase64(const std::string &base64, const v_bytes &key) {
        const auto bytes = Base64Decode(base64);
        return Decrypt(bytes, key);
    }

    v_bytes Encryption::DecryptBase64Bin(const std::string &base64, const v_bytes &key) {
        const auto bytes = Base64Decode(base64);
        return DecryptToBin(bytes, key);
    }

    v_bytes Encryption::GenerateIV() {
        v_bytes IV(EVP_MAX_IV_LENGTH);
        if (!RAND_bytes(IV.data(), EVP_MAX_IV_LENGTH)) {
            Error(ErrorType::Fatal, "Failed to generate IV").Dispatch();
        }
        return IV;
    }

    std::string Encryption::Base64Encode(const v_bytes &binary) {
        BUF_MEM *buffer_ptr;

        BIO *b64 = BIO_new(BIO_f_base64());
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
        BIO *bio = BIO_new(BIO_s_mem());
        bio = BIO_push(b64, bio);

        BIO_write(bio, binary.data(), static_cast<int>(binary.size()));
        BIO_flush(bio);
        BIO_get_mem_ptr(bio, &buffer_ptr);

        std::string result(buffer_ptr->data, buffer_ptr->length);

        BIO_free_all(bio);

        return result;
    }

    v_bytes Encryption::Base64Decode(const std::string &base64_data) {
        v_bytes result(base64_data.size());

        BIO *base64 = BIO_new(BIO_f_base64());
        BIO_set_flags(base64, BIO_FLAGS_BASE64_NO_NL);
        BIO *bio = BIO_new_mem_buf(base64_data.data(), -1);
        bio = BIO_push(base64, bio);

        const int decoded_size = BIO_read(bio, result.data(), static_cast<int>(base64_data.size()));
        BIO_free_all(bio);
        if (decoded_size < 0) {
            Error(ErrorType::Fatal, "Failed to decode base64 data").Dispatch();
        }

        result.resize(decoded_size);
        return result;
    }
} // namespace Infinity
