
#pragma once

#include "imgui.h"


#include "ImGuiFileDialog.h"
#include "Util/Encryption/Encryption.hpp"

#include <vector>
#include <string>

namespace Infinity {
    class SelectKeyFromFile {
public:
        SelectKeyFromFile();

        void Render();

        private:
        void LoadKeyFromFile();

        private:
        bool m_RememberKeyFromFile;
        std::vector<uint8_t> m_KeyData;
        std::string m_FilePath;
    };

}
