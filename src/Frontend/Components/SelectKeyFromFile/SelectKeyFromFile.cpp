
#include "SelectKeyFromFile.hpp"
#include "Backend/BinaryFiles/BinaryFiles.hpp"
#include "Backend/PersistentState/PersistentState.hpp"
#include "Util/Encryption/Encryption.hpp"
#include "Util/Errors/Error.hpp"

#include <fstream>
#include <iostream>


namespace Infinity {
    SelectKeyFromFile::SelectKeyFromFile() : m_RememberKeyFromFile(false), m_KeyData() {}

    std::string SelectKeyFromFile::LoadKey() {
        std::filesystem::path config_path;

#ifdef WIN32
        const *appdata_path = std::getenv("APPDATA");
        if (!appdata_path) {
            std::cerr << "Error: Unable to determine AppData folder (APPDATA environment variable is not set)." << std::endl;
        }
        config_path = std::filesystem::path(appdata_path) / "infinityPackageBuilder";
#elif __linux__
        char *home_dir = std::getenv("HOME");
        if (!home_dir) {
            std::cerr << "Error: Unable to determine home directory (HOME environment variable is not set)." << std::endl;
        }
        config_path = std::filesystem::path(home_dir) / ".config/infinityPackageBuilder";
#else
        std::cerr << "Error: Unsupported platform." << std::endl;
#endif

        config_path += "/group.infinitystate";

        std::ifstream file(config_path);
        if (!file.is_open()) {
            Error(ErrorType::NonFatal, "Failed to open keyfile: " + config_path.string()).Dispatch();
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    void SelectKeyFromFile::LoadKeyFromFile() {
        std::ifstream key_file(m_FilePath, std::ios::binary | std::ios::ate);
        if (!key_file.is_open()) {
            Error(ErrorType::NonFatal, "Failed to open key file").Dispatch();
        }

        std::streamsize file_size = key_file.tellg();
        if (file_size <= 0) {
            Error(ErrorType::NonFatal, "Failed to read key file").Dispatch();
        }

        std::vector<uint8_t> key_buffer(file_size);

        key_file.seekg(0, std::ios::beg);
        if (!key_file.read(reinterpret_cast<char *>(key_buffer.data()), file_size)) {
            Error(ErrorType::NonFatal, "Failed to read key file").Dispatch();
        }

        m_KeyData = key_buffer;
    }

    void SelectKeyFromFile::Render() {

        auto persistent_state = PersistentState::GetInstance();
        auto &state = persistent_state->GetState();
        m_RememberKeyFromFile = state.remember_key;


        if (m_RememberKeyFromFile && m_KeyData.empty()) {
            auto key = LoadKey();
            auto key_vec = Encryption::Base64Decode(key);
            m_KeyData = key_vec;
        }

        ImGui::Text("Please select your prefered method for submiting your access key:");
        if (ImGui::Button("Load Key From Clipboard (base64 encoded codes)")) {
        }
        if (ImGui::Button("Load Key From .infinitykey file")) {
            ShowFileDialog(m_FilePath);
        }

        SaveKey();


        if (ImGui::Checkbox("Remember Key Data", &m_RememberKeyFromFile)) {
            state.remember_key = m_RememberKeyFromFile;
            persistent_state->SaveState();
        }


        ImGui::Text("Using key from file = %s", m_FilePath.c_str());
        ImGui::Text("Key Data = %s", m_KeyData.data());
    }

    void SelectKeyFromFile::SaveKey() {

        if (!m_RememberKeyFromFile || m_PreviousTickFilePath == m_FilePath) {
            m_PreviousTickFilePath = m_FilePath;
            return;
        }

        if (EndsWith(m_FilePath, ".infinitykey")) {
            std::vector<uint8_t> key_bin = ReadBinaryFile(m_FilePath);
            auto base64key = Encryption::Base64Encode(key_bin);

            std::filesystem::path output_path;

#ifdef WIN32
            const *appdata_path = std::getenv("APPDATA");
            if (!appdata_path) {
                std::cerr << "Error: Unable to determine AppData folder (APPDATA environment variable is not set)." << std::endl;
                return;
            }
            output_path = std::filesystem::path(appdata_path) / "infinityPackageBuilder";
#elif __linux__
            char *home_dir = std::getenv("HOME");
            if (!home_dir) {
                std::cerr << "Error: Unable to determine home directory (HOME environment variable is not set)." << std::endl;
                return;
            }
            output_path = std::filesystem::path(home_dir) / ".config/infinityPackageBuilder";
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
