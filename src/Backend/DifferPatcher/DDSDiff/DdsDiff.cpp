
#include "DdsDiff.hpp"

#include <fstream>
#include <iostream>

#include <minizip/zip.h>
#include "Util/Errors/Error.hpp"

namespace Infinity {
    std::pair<std::vector<uint8_t>, std::vector<uint8_t>> DDSDiff::ReadDDS(const std::string &file_name) {
        std::ifstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            Error(ErrorType::Warning, "Failed to open file `" + file_name + "`").Dispatch();
        }
        std::vector<uint8_t> header(128);
        file.read(reinterpret_cast<char *>(header.data()), static_cast<std::streamsize>(header.size()));
        if (file.gcount() != static_cast<std::streamsize>(header.size())) {
            Error(ErrorType::Warning, "Failed to read DDS file header").Dispatch();
        }
        std::vector<uint8_t> data((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
        return {header, data};
    }

    void DDSDiff::SaveDDS(const std::string &file_name, const std::vector<uint8_t> &header, const std::vector<uint8_t> &data) {
        std::ofstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            Error(ErrorType::Warning, "Failed to open file `" + file_name + "`").Dispatch();
        }

        file.write(reinterpret_cast<const char *>(header.data()), static_cast<std::streamsize>(header.size()));
        file.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
    }

    void DDSDiff::SaveDiff(const std::string &file_name, const std::vector<uint8_t> &diff) {
        [[maybe_unused]] std::ofstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            Error(ErrorType::Warning, "Failed to create file `" + file_name + "`").Dispatch();
        }

        std::vector<uint8_t> compressed(static_cast<std::streamsize>(static_cast<double>(diff.size()) * 1.1 + 12));
        uLong compressed_size = compressed.size();

        if (const int res = compress(compressed.data(), &compressed_size, diff.data(), diff.size()); res != Z_OK) {
            Error(ErrorType::Warning, "Failed to compress diff").Dispatch();
        }

        compressed.resize(compressed_size);
        file.write(reinterpret_cast<const char *>(compressed.data()), static_cast<std::streamsize>(compressed.size()));
    }

    std::vector<uint8_t> DDSDiff::ComputeDiff(const std::vector<uint8_t> &image1, const std::vector<uint8_t> &image2) {
        std::vector<uint8_t> diff(image1.size());
        std::transform(image2.begin(), image2.end(), image1.begin(), diff.begin(), [](const uint8_t b2, const uint8_t b1) { return b2 - b1; });

        return diff;
    }

    std::vector<uint8_t> DDSDiff::Diff(const std::string &file_a, const std::string &file_b) {
        auto [header1, data1] = ReadDDS(file_a);
        auto [header2, data2] = ReadDDS(file_b);

        return ComputeDiff(data1, data2);
    }

    void DDSDiff::PatchImage(const std::vector<uint8_t> &originalHeader, const std::vector<uint8_t> &originalImage, const std::vector<uint8_t> &diff, const std::string &output) {
        std::vector<uint8_t> patched(originalImage.size());
        std::transform(originalImage.begin(), originalImage.end(), diff.begin(), patched.begin(), [](const uint8_t pixel, const uint8_t l_diff) { return pixel + l_diff; });
        SaveDDS(output, originalHeader, patched);
    }

    std::vector<uint8_t> DDSDiff::DecompressPatchFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            Error(ErrorType::Warning, "Failed to open patch file: " + filename).Dispatch();
        }

        const std::vector<uint8_t> compressedData((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());

        uLong uncompressedSize = compressedData.size() * 10;
        std::vector<uint8_t> decompressedData(uncompressedSize);

        if (const int result = uncompress(decompressedData.data(), &uncompressedSize, compressedData.data(), compressedData.size()); result != Z_OK) {
            Error(ErrorType::Warning, "Failed to decompress patch file").Dispatch();
        }

        decompressedData.resize(uncompressedSize);

        return decompressedData;
    }
} // namespace Infinity
