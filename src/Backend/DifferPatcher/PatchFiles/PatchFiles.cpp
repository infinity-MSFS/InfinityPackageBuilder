
#include "PatchFiles.hpp"
#include "Backend/DifferPatcher/DiffPatch.hpp"

#include <fstream>

#include "Util/Errors/Error.hpp"

namespace Infinity {
    void PatchFiles::PatchFile(const std::string &file, const std::vector<uint8_t> &patch, const std::string &temp_file_path) {
        std::string temp_file_name = temp_file_path + gen_random(10) + ".infinitypatch";
        create_temp_file(temp_file_name.c_str(), patch.data(), patch.size());
        std::string temp_file_content = temp_file_path + file + ".infinitytemp";
        create_temp_file(temp_file_content.c_str(), {}, {});
        if (apply_patch(file.c_str(), temp_file_content.c_str(), temp_file_name.c_str())) {
            Error(ErrorType::Warning, "Failed to apply temporary patch file").Dispatch();
            return;
        }

        remove(file.c_str());
        rename(temp_file_content.c_str(), file.c_str());
        remove(temp_file_name.c_str());
    }

    void PatchFiles::PatchViaMap(PatchMapStructure map, PatchBinary patch, const std::string &aircraft_folder, const std::string &temp_file_path) {
        for (auto &relative_path: map.changed_files) {
            std::string file_path = aircraft_folder + "/" + relative_path;

            auto it = patch.find(relative_path);
            if (it == patch.end()) {
                std::vector<uint8_t> &patch_data = it->second;
                PatchFile(file_path, patch_data, temp_file_path);
            } else {
                std::cerr << "Patch data not found for file: " << relative_path << "\n";
            }
        }
    }

    FileStructure PatchFiles::ParsePatchFiles(const std::string &path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            Error(ErrorType::Warning, "Failed to open file: " + path).Dispatch();
            return {};
        }
        std::vector<char> buffer((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());

        msgpack::object_handle oh = msgpack::unpack(buffer.data(), buffer.size());

        msgpack::object obj = oh.get();
        FileStructure file_structure;
        obj.convert(file_structure);


        return file_structure;
    }


} // namespace Infinity
