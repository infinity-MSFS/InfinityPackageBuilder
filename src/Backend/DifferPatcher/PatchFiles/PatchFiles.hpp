
#pragma once

#include "Backend/DifferPatcher/PatchBinaryStructure.hpp"

#include "msgpack.hpp"


namespace Infinity {

    class PatchFiles {

    public:
        static void PatchViaMap(const PatchMapStructure& map, PatchBinary patch, const std::string &aircraft_folder, const std::string &temp_file_path);
        static FileStructure ParsePatchFiles(const std::string &path);

    private:
        static void PatchFile(const std::string &file, const std::vector<uint8_t> &patch, const std::string &temp_file_path);
    };
} // namespace Infinity
