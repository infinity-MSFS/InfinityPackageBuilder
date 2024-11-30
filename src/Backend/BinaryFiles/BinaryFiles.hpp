
#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <vector>

#include "Util/Errors/Error.hpp"

namespace Infinity {

    inline std::vector<uint8_t> ReadBinaryFile(const std::filesystem::path &file_path) {
        std::ifstream file(file_path, std::ios::binary | std::ios::ate);
        if (!file)
            Error(ErrorType::NonFatal, "Failed to open file: " + file_path.string()).Dispatch();

        const std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<uint8_t> buffer(size);
        if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
            Error(ErrorType::NonFatal, "Error reading file: " + file_path.string()).Dispatch();

        return buffer;
    }
} // namespace Infinity
