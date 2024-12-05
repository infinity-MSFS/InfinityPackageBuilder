
#pragma once
#include <string>
#include <utility>
#include <vector>


namespace Infinity {
    class DDSDiff {

    public:
        static std::vector<uint8_t> Diff(const std::string &file_a, const std::string &file_b);

        static void PatchImage(const std::vector<uint8_t> &originalHeader, const std::vector<uint8_t> &originalImage, const std::vector<uint8_t> &diff, const std::string &output);

    private:
        static std::pair<std::vector<uint8_t>, std::vector<uint8_t>> ReadDDS(const std::string &file_name);
        static void SaveDDS(const std::string &file_name, std::vector<uint8_t> header, std::vector<uint8_t> data);
        [[maybe_unused]] static void SaveDiff(const std::string &file_name, const std::vector<uint8_t> &diff);
        [[maybe_unused]] static std::vector<uint8_t> DecompressPatchFile(const std::string &filename);
        static std::vector<uint8_t> ComputeDiff(const std::vector<uint8_t> image1, const std::vector<uint8_t> image2);
    };
} // namespace Infinity
