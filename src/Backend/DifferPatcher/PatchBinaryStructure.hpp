
#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "msgpack.hpp"


namespace Infinity {
    struct HashOutput {
        std::string fileName;
        std::string hash;

        MSGPACK_DEFINE(fileName, hash);
    };

    struct FileStructure {
        std::string version;
        std::vector<HashOutput> files;

        MSGPACK_DEFINE(version, files);
    };

    struct PatchMapStructure {
        std::pair<std::string, std::string> version;
        std::vector<std::string> changed_files;
        std::vector<std::string> removed_files;
        std::vector<std::string> added_files;

        MSGPACK_DEFINE(version, changed_files, removed_files, added_files);

        PatchMapStructure(const std::string &old_version, const std::string &new_version) : version(old_version, new_version) {}
    };

    typedef std::unordered_map<std::string, std::vector<uint8_t>> PatchBinary;


} // namespace Infinity
