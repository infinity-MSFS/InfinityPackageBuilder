
#include "Config.hpp"

#include <iostream>

namespace Infinity {

    Config *Config::m_Instance = nullptr;


    AppConfig::AppConfig() : cache_path(""), key_location(""), lua_release(false) {}

    Config::Config() { LoadConfig(); }

    Config &Config::getInstance() {
        if (!m_Instance) {
            m_Instance = new Config();
        }
        return *m_Instance;
    }

    Config::~Config() {
        delete m_Instance;
        m_Instance = nullptr;
    }

    AppConfig Config::GetConfig() { return m_AppConfig; }

    void Config::LoadConfig() {
        m_ConfigPath = GetFilePath() + "/app_config.ini";
        if (!exists(m_ConfigPath)) {
            SaveConfig();
        }

        const mINI::INIFile file(m_ConfigPath.string());
        mINI::INIStructure ini;
        file.read(ini);

        m_AppConfig.cache_path = ini["General"]["CachePath"].c_str();
        m_AppConfig.key_location = ini["General"]["KeyLocation"].c_str();
        m_AppConfig.lua_release = ini["General"]["LuaRelease"] == "true";
    }

    std::string Config::GetFilePath() {

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

    void Config::SaveConfig() const {
        mINI::INIStructure ini;

        ini["General"]["CachePath"] = m_AppConfig.cache_path;
        ini["General"]["KeyLocation"] = m_AppConfig.key_location;
        ini["General"]["LuaRelease"] = m_AppConfig.lua_release ? "true" : "false";

        const mINI::INIFile file(m_ConfigPath.string());
        file.write(ini);
    }

    void Config::UpdateConfig(const AppConfig &config) { m_AppConfig = config; }


} // namespace Infinity
