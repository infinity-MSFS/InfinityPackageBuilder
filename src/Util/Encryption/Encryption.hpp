
#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Util/Errors/Error.hpp"
#include "openssl/aes.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"
#include "openssl/evp.h"
#include "openssl/rand.h"

#include "admin_key.h"
#include "aero_dynamics_key.h"
#include "client_key.h"
#include "delta_sim_key.h"
#include "github_key.h"
#include "group_key.h"
#include "lunar_sim_key.h"
#include "ouroboros_key.h"
#include "qbit_sim_key.h"

#ifdef WIN32
#include <Windows.h>
#include <iostream>
#endif

namespace Infinity {
    typedef std::vector<uint8_t> v_bytes;


#ifdef WIN32


    inline bool HideFile(const std::string &filePath) {
        if (SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_HIDDEN) == 0) {
            std::cerr << "Error: Could not hide the file. Error code: " << GetLastError() << std::endl;
            return false;
        }
        return true;
    }
#endif


    enum class Keys { Admin, AeroDynamics, Client, DeltaSim, Github, Group, LunarSim, Ouroboros, QbitSim, Unknown };
    enum class ValidationTypes { FULL, AERO_DYNAMICS, DELTA_SIM, LUNAR_SIM, OUROBOROS, QBIT_SIM, NOT_AUTHORIZED };
    struct Key {
        v_bytes key;
        Keys name;


        static ValidationTypes GetValidationType(const Key &provided_key , std::unordered_map<Keys, v_bytes>key_map);
        static v_bytes ParseKeyFile(const std::string &file_path);
        static std::unordered_map<Keys, v_bytes> GetKeyMap();
    };

    class Encryption {
    public:
        static v_bytes Encrypt(const std::string &plain_text, const v_bytes &key);
        static v_bytes Encrypt(const v_bytes &binary, const v_bytes &key);
        static std::string EncryptBase64(const std::string &plain_text, const v_bytes &key);
        static std::string EncryptBase64(const v_bytes &binary, const v_bytes &key);

        static std::string Decrypt(const v_bytes &binary, const v_bytes &key);
        static v_bytes DecryptToBin(const v_bytes &binary, const v_bytes &key);
        static std::string DecryptBase64(const std::string &base64, const v_bytes &key);
        static v_bytes DecryptBase64Bin(const std::string &base64, const v_bytes &key);

        static std::string Base64Encode(const v_bytes &binary);

        static v_bytes Base64Decode(const std::string &base64_data);

    private:
        static v_bytes GenerateIV();
    };

    inline v_bytes CreateUnencryptedKey(const unsigned char *raw_binary) {
        v_bytes key(raw_binary, raw_binary + sizeof(raw_binary));
        return key;
    }

    inline v_bytes CreateKey(const unsigned char *raw_binary, v_bytes key) {
        const v_bytes encrypted_key(raw_binary, raw_binary + sizeof(raw_binary));
        auto result = Encryption::DecryptToBin(encrypted_key, key);
        return result;
    }
} // namespace Infinity
