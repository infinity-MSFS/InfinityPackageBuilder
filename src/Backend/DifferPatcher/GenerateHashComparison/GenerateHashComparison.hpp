
#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>


#include "Backend/DifferPatcher/PatchBinaryStructure.hpp"

namespace Infinity {
    class GenerateHashComparison {

    public:
        static void HashAircraft(const std::string &path, const std::string &version, const std::string &output_path);
        static void CompareHash(const std::string &old_path, const std::string &new_path, const std::string &output_path);


    private:
        static std::string ComputeHash(const std::filesystem::path &file_path);
        static void HashFile(const std::filesystem::path &relative_path, const std::filesystem::path &file_path, FileStructure &file_hashes, std::mutex &mutex);
    };


} // namespace Infinity
