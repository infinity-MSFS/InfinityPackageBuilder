
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
        file.read(reinterpret_cast<char *>(header.data()), header.size());
        if (file.gcount() != static_cast<std::streamsize>(header.size())) {
            Error(ErrorType::Warning, "Failed to read DDS file header").Dispatch();
        }
        std::vector<uint8_t> data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return {header, data};
    }

    void DDSDiff::SaveDDS(const std::string &file_name, std::vector<uint8_t> header, std::vector<uint8_t> data) {
        std::ofstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create file: " + file_name);
        }

        file.write(reinterpret_cast<const char *>(header.data()), header.size());
        file.write(reinterpret_cast<const char *>(data.data()), data.size());
    }

    void DDSDiff::SaveDiff(const std::string &file_name, const std::vector<uint8_t> &diff) {
        [[maybe_unused]] std::ofstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to create file: " + file_name);
        }

        std::vector<uint8_t> compressed(diff.size() * 1.1 + 12);
        uLong compressed_size = compressed.size();

        int res = compress(compressed.data(), &compressed_size, diff.data(), diff.size());
        if (res != Z_OK) {
            throw std::runtime_error("Failed to compress diff");
        }

        compressed.resize(compressed_size);
        file.write(reinterpret_cast<const char *>(compressed.data()), compressed.size());
    }

    std::vector<uint8_t> DDSDiff::ComputeDiff(const std::vector<uint8_t> image1, const std::vector<uint8_t> image2) {
        if (image1.size() != image2.size()) {
            throw std::invalid_argument("Images must have the same size");
        }

        std::vector<uint8_t> diff(image1.size());
        std::transform(image2.begin(), image2.end(), image1.begin(), diff.begin(), [](uint8_t b2, uint8_t b1) { return b2 - b1; });

        return diff;
    }

    std::vector<uint8_t> DDSDiff::Diff(const std::string &file_a, const std::string &file_b) {
        auto [header1, data1] = ReadDDS(file_a);
        auto [header2, data2] = ReadDDS(file_b);

        return ComputeDiff(data1, data2);
    }

    void DDSDiff::PatchImage(const std::vector<uint8_t> &originalHeader, const std::vector<uint8_t> &originalImage, const std::vector<uint8_t> &diff, const std::string &output) {
        std::vector<uint8_t> patched(originalImage.size());
        std::transform(originalImage.begin(), originalImage.end(), diff.begin(), patched.begin(), [](uint8_t pixel, uint8_t diff) { return pixel + diff; });
        SaveDDS(output, originalHeader, patched);
    }

    std::vector<uint8_t> DDSDiff::DecompressPatchFile(const std::string &filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open patch file: " + filename);
        }

        std::vector<uint8_t> compressedData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        uLong uncompressedSize = compressedData.size() * 10;
        std::vector<uint8_t> decompressedData(uncompressedSize);

        int result = uncompress(decompressedData.data(), &uncompressedSize, compressedData.data(), compressedData.size());
        if (result != Z_OK) {
            throw std::runtime_error("Failed to decompress patch file");
        }

        decompressedData.resize(uncompressedSize);

        return decompressedData;
    }


} // namespace Infinity
