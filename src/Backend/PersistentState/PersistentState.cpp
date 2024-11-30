
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
        const *appdata_path = std::getenv("APPDATA");
        if (!appdata_path) {
            std::cerr << "Error: Unable to determine AppData folder (APPDATA environment variable is not set)." << std::endl;
        }
        output_path = std::filesystem::path(appdata_path) / "infinityPackageBuilder";
#elif __linux__
        char *home_dir = std::getenv("HOME");
        if (!home_dir) {
            std::cerr << "Error: Unable to determine home directory (HOME environment variable is not set)." << std::endl;
        }
        output_path = std::filesystem::path(home_dir) / ".config/infinityPackageBuilder";
#else
        std::cerr << "Error: Unsupported platform." << std::endl;
#endif
        return output_path.string();
    }
} // namespace Infinity
