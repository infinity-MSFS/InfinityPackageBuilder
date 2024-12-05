
#pragma once

#include <chrono>
#include <vector>

#include "Backend/DifferPatcher/PatchBinaryStructure.hpp"
#include "bsdiff.h"

namespace Infinity {


    class DiffFiles {

    public:
        static void DiffFromMap(const PatchMapStructure &map_structure, const std::string &aircraft_folder_a, const std::string &aircraft_folder_b, const std::string &output_path,
                                const std::string &temp_directory);

    private:
        static std::string TrimRelativePath(const std::string &relative_path);

        static std::vector<uint8_t> Diff(const std::string &file_a, const std::string &file_b, const std::string &temp_directory);

        static void SpawnDiffTask(const std::string &aircraft_folder_a, const std::string &aircraft_folder_b, const std::string &output_path, const std::string &relative_path,
                                  const std::string &temp_directory);
    };

} // namespace Infinity
