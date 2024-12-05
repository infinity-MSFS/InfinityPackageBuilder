
#include "DiffFiles.hpp"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

#include "Backend/DifferPatcher/DDSDiff/DdsDiff.hpp"
#include "Backend/DifferPatcher/DiffPatch.hpp"
#include "Util/Errors/Error.hpp"

namespace Infinity {


    std::string DiffFiles::TrimRelativePath(const std::string &relative_path) {
        if (const size_t last_slash_index = relative_path.find_last_of('\\'); last_slash_index != std::string::npos) {
            return relative_path.substr(last_slash_index + 1);
        }
        return relative_path;
    }

    std::vector<uint8_t> DiffFiles::Diff(const std::string &file_a, const std::string &file_b, const std::string &temp_directory) {
        if (file_a.size() > 4 && file_b.size() > 4 && file_a.substr(file_a.size() - 4) == ".dds" && file_b.substr(file_b.size() - 4) == ".dds") {
            return DDSDiff::Diff(file_a, file_b);
        }
        srand(static_cast<unsigned>(time(nullptr)) * _getpid());

        const std::string patch_file_name = temp_directory + "/" + gen_random(10) + ".infinitypatch";
        if (generate_patch(file_a.c_str(), file_b.c_str(), patch_file_name.c_str()) != BSDIFF_SUCCESS) {
            fprintf(stderr, "Failed to generate diff file %s\n", patch_file_name.c_str());
            return {};
        }

        std::ifstream patch_file(patch_file_name, std::ios::binary);

        std::vector<uint8_t> patch_data((std::istreambuf_iterator(patch_file)), std::istreambuf_iterator<char>());

        patch_file.close();

        return patch_data;
    }

    void DiffFiles::SpawnDiffTask(const std::string &aircraft_folder_a, const std::string &aircraft_folder_b, const std::string &output_path, const std::string &relative_path,
                                  const std::string &temp_directory) {
        const std::string file_path_a = aircraft_folder_a + "/" + relative_path;
        const std::string file_path_b = aircraft_folder_b + "/" + relative_path;

        const auto start_time = std::chrono::high_resolution_clock::now();

        const std::string output_file = output_path + "/" + TrimRelativePath(relative_path);

        const std::vector<uint8_t> patch = Diff(file_path_a, file_path_b, temp_directory);
        std::ofstream out(output_file, std::ios::binary);
        out.write(reinterpret_cast<const char *>(patch.data()), static_cast<std::streamsize>(patch.size()));

        const auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
        const std::chrono::duration<double> duration = elapsed_time;
        std::cout << "Time taken for " << relative_path << ": " << duration.count() << " seconds" << std::endl;
    }

    void DiffFiles::DiffFromMap(const PatchMapStructure &map_structure, const std::string &aircraft_folder_a, const std::string &aircraft_folder_b, const std::string &output_path,
                                const std::string &temp_directory) {
        std::vector<std::thread> diff_threads;

        for (const auto &relative_path: map_structure.changed_files) {
            diff_threads.emplace_back([=] { SpawnDiffTask(aircraft_folder_a, aircraft_folder_b, output_path, relative_path, temp_directory); });
        }

        for (auto &t: diff_threads) {
            t.detach();
        }
    }
} // namespace Infinity
