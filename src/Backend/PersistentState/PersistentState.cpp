
#include "PersistentState.hpp"

namespace Infinity {
    PersistentState *PersistentState::m_Instance = nullptr;

    void PersistentState::SaveState() {
        if (m_AppState != m_PreviousState) {
            auto output_path = GetFilePath();
            std::filesystem::create_directories(output_path);
            output_path += "/settings.infinitystate";
            nlohmann::json json = m_AppState;
            std::ofstream file(output_path);
            file << json.dump(4);

            std::cout << "Saved state to: " << output_path << std::endl;

            m_PreviousState = m_AppState;
        }
    }

    void PersistentState::LoadState() {
        auto file_name = GetFilePath();
        file_name += "/settings.infinitystate";
        std::ifstream file(file_name);
        if (file) {
            nlohmann::json json;
            file >> json;
            m_AppState = json.get<PersistentStateStruct>();

            m_PreviousState = m_AppState;
        }
    }

    std::string PersistentState::GetFilePath() {
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
#endif
        return output_path.string();
    }
} // namespace Infinity
