
#include "GenerateHashComparison.hpp"
#include "Util/Errors/Error.hpp"

#include <iomanip>
#include <iostream>

#include <ranges>
#include <sstream>

#include <thread>
#include <vector>

#include "msgpack.hpp"
#include "openssl/evp.h"

namespace Infinity {

    std::string GenerateHashComparison::ComputeHash(const std::filesystem::path &file_path) {
        std::ifstream file(file_path, std::ios::binary);
        if (!file)
            Error(ErrorType::Warning, "Unable to open hash fil: " + file_path.string()).Dispatch();

        EVP_MD_CTX *ctx = EVP_MD_CTX_new();
        if (!ctx) {
            Error(ErrorType::Warning, "Failed to create EVP_MD_CTX").Dispatch();
        }

        if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
            EVP_MD_CTX_free(ctx);
            Error(ErrorType::Warning, "Failed to initialize digest context").Dispatch();
        }

        char buffer[65536];
        while (file.read(buffer, sizeof(buffer)) || file.gcount()) {
            if (EVP_DigestUpdate(ctx, buffer, file.gcount()) != 1) {
                EVP_MD_CTX_free(ctx);
                Error(ErrorType::Warning, "Failed to update digest").Dispatch();
            }
        }

        unsigned char hash[EVP_MAX_MD_SIZE];
        unsigned int hash_length = 0;
        if (EVP_DigestFinal_ex(ctx, hash, &hash_length) != 1) {
            EVP_MD_CTX_free(ctx);
            Error(ErrorType::Warning, "Failed to finalize digest").Dispatch();
        }

        EVP_MD_CTX_free(ctx);

        std::ostringstream hash_string;
        for (unsigned int i = 0; i < hash_length; ++i) {
            hash_string << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
        }

        return hash_string.str();
    }

    void GenerateHashComparison::HashFile(const std::filesystem::path &relative_path, const std::filesystem::path &file_path, FileStructure &file_hashes, std::mutex &mutex) {
        try {
            const std::string hash = ComputeHash(file_path);
            const HashOutput file_hash = {relative_path.string(), hash};

            std::lock_guard lock(mutex);
            file_hashes.files.push_back(file_hash);
        } catch (const std::exception &e) {
            Error(ErrorType::Warning, "Error handling file: " + file_path.string() + ": " + e.what()).Dispatch();
        }
    }

    void GenerateHashComparison::HashAircraft(const std::string &path, const std::string &version, const std::string &output_path) {
        std::thread([=] {
            FileStructure file_hashes{version, {}};
            std::mutex mutex;
            std::vector<std::thread> threads;

            for (const auto &entry: std::filesystem::recursive_directory_iterator(path)) {
                if (entry.is_regular_file()) {
                    std::filesystem::path relative_path = relative(entry.path(), path);
                    threads.emplace_back(HashFile, relative_path, version, output_path, mutex);
                }
            }

            for (auto &thread: threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }

            try {
                std::ofstream output(output_path + "/hash.infinityhash", std::ios::binary);
                if (!output) {
                    Error(ErrorType::Warning, "Unable to create output file: " + output_path).Dispatch();
                }
                msgpack::pack(output, file_hashes);
                std::cout << "Hashing completed and saved to " << output_path + "/hash.infinityhash" << '\n';
            } catch (const std::exception &e) {
                std::string error = e.what();
                Error(ErrorType::Warning, "Error saving hash file: " + error).Dispatch();
            }
        }).detach();
    }

    void GenerateHashComparison::CompareHash(const std::string &old_path, const std::string &new_path, const std::string &output_path) {
        std::thread([=] {
            try {
                std::ifstream old_file(old_path, std::ios::binary);
                std::ifstream new_file(new_path, std::ios::binary);
                if (!old_file || !new_file) {
                    Error(ErrorType::Warning, "Unable to open one of the input files.").Dispatch();
                }

                std::stringstream old_buffer, new_buffer;
                old_buffer << old_file.rdbuf();
                new_buffer << new_file.rdbuf();

                auto old_handle = msgpack::unpack(old_buffer.str().data(), old_buffer.str().size());
                auto new_handle = msgpack::unpack(new_buffer.str().data(), new_buffer.str().size());

                auto [version_old, files_old] = old_handle.get().as<FileStructure>();
                auto [version_new, files_new] = new_handle.get().as<FileStructure>();

                PatchMapStructure output_map(version_old, version_new);


                for (const auto &old_entry: files_old) {

                    if (auto it = std::ranges::find_if(files_new.begin(), files_new.end(), [&old_entry](const HashOutput &new_entry) { return new_entry.fileName == old_entry.fileName; });
                        it != files_new.end()) {
                        if (it->hash != old_entry.hash) {
                            output_map.changed_files.push_back(it->fileName);
                        }
                    } else {
                        output_map.removed_files.push_back(old_entry.fileName);
                    }
                }

                for (const auto &new_entry: files_new) {

                    if (auto it = std::ranges::find_if(files_old.begin(), files_old.end(), [&new_entry](const HashOutput &old_entry) { return old_entry.fileName == new_entry.fileName; });
                        it == files_old.end()) {
                        output_map.added_files.push_back(new_entry.fileName);
                    }
                }

                std::ofstream output_file(output_path + "map.infinitymap", std::ios::binary);
                if (!output_file) {
                    Error(ErrorType::Warning, "Failed to create output file: " + output_path + "/map.infinitymap").Dispatch();
                }
                msgpack::pack(output_file, output_map);
                std::cout << "Comparison complete and wrote to: " << output_path + "map.infinitymap" << '\n';
            } catch (const std::exception &e) {
                Error(ErrorType::Warning, "Error during comparison: " + std::string(e.what())).Dispatch();
            }
        }).detach();
    }


} // namespace Infinity
