
#include "SelectKeyFromFile.hpp"
#include "Backend/BinaryFiles/BinaryFiles.hpp"
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
        if (file_size != 64) {
            Error(ErrorType::NonFatal, "Failed to read key file size").Dispatch();
        }

        std::vector<uint8_t> key_buffer(file_size);

        key_file.seekg(0, std::ios::beg);
        if (!key_file.read(reinterpret_cast<char *>(key_buffer.data()), file_size)) {
            Error(ErrorType::NonFatal, "Failed to read key file").Dispatch();
        }

        m_KeyData = Encryption::DecryptToBin(key_buffer, {group_key, group_key + sizeof(group_key)});
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


    std::optional<std::vector<uint8_t>> SelectKeyFromFile::GetKey() {
        if (!m_KeyData.empty()) {
            return m_KeyData;
        }
        return std::nullopt;
    }


} // namespace Infinity
