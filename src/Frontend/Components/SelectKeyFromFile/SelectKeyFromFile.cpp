
#include "SelectKeyFromFile.hpp"
#include "Backend/BinaryFiles/BinaryFiles.hpp"
#include "Backend/PersistentState/PersistentState.hpp"
#include "Util/Encryption/Encryption.hpp"
#include "Util/Errors/Error.hpp"

#include <fstream>
#include <iostream>


namespace Infinity {
    SelectKeyFromFile::SelectKeyFromFile() : m_RememberKeyFromFile(false), m_KeyData() {}



    void SelectKeyFromFile::LoadKeyFromFile() {
        std::ifstream key_file(m_FilePath, std::ios::binary | std::ios::ate);
        if (!key_file.is_open()) {
            Error(ErrorType::NonFatal, "Failed to open key file").Dispatch();
        }

        std::streamsize file_size = key_file.tellg();
        if (file_size <= 0) {
            Error(ErrorType::NonFatal, "Failed to read key file size").Dispatch();
        }

        std::vector<uint8_t> key_buffer(file_size);

        key_file.seekg(0, std::ios::beg);
        if (!key_file.read(reinterpret_cast<char *>(key_buffer.data()), file_size)) {
            Error(ErrorType::NonFatal, "Failed to read key file").Dispatch();
        }

        m_KeyData = key_buffer;
    }

    void SelectKeyFromFile::Render() {



        ImGui::Text("Please select your prefered method for submiting your access key:");
        if (ImGui::Button("Load Key From Clipboard (base64 encoded codes)")) {
        }
        if (ImGui::Button("Load Key From .infinitykey file")) {
            ShowFileDialog(m_FilePath);
        }

        if (m_KeyData.empty() && !m_FilePath.empty()) {
            LoadKeyFromFile();
        }



        ImGui::Text("Using key from file = %s", m_FilePath.c_str());
        ImGui::Text("Key Data = %s", m_KeyData.data());
    }

    void SelectKeyFromFile::SaveKey() {

        if (m_PreviousTickFilePath == m_FilePath) {
            m_PreviousTickFilePath = m_FilePath;
            return;
        }

        if (EndsWith(m_FilePath, ".infinitykey")) {
            std::vector<uint8_t> key_bin = ReadBinaryFile(m_FilePath);
            auto base64key = Encryption::Base64Encode(key_bin);

            std::filesystem::path output_path;

#ifdef WIN32
            char *appdata_path = nullptr;
            size_t len = 0;
            if (_dupenv_s(&appdata_path, &len, "APPDATA") != 0 || appdata_path == nullptr) {
                std::cerr << "Error: Unable to determine AppData folder (APPDATA environment variable is not set)." << std::endl;
            } else {
                output_path = std::filesystem::path(appdata_path) / "infinityPackageBuilder";
                free(appdata_path);
            }
#elif __linux__
            char *home_dir = nullptr;
            size_t len = 0;
            if (_dupenv_s(&home_dir, &len, "HOME") != 0 || home_dir == nullptr) {
                std::cerr << "Error: Unable to determine home directory (HOME environment variable is not set)." << std::endl;
            } else {
                output_path = std::filesystem::path(home_dir) / ".config/infinityPackageBuilder";
                free(home_dir);
            }
#else
            std::cerr << "Error: Unsupported platform." << std::endl;
            return;
#endif
            std::filesystem::create_directories(output_path);

            output_path /= "group.infinitystate";
            std::ofstream outFile(output_path, std::ios::out | std::ios::trunc);
            if (outFile.is_open()) {
                outFile << base64key;
                outFile.close();
                std::cout << "Wrote key to state file" << output_path << std::endl;
            } else {
                Error(ErrorType::NonFatal, "Failed to create .infinitystate file in: " + output_path.string()).Dispatch();
            }
        }
        m_PreviousTickFilePath = m_FilePath;
    }

} // namespace Infinity
