
#pragma once

#include <filesystem>
#include <string>

#include "Ini/ini.h"

// Im going to make it save the config upon clicking the x button so alt+f4 or end task wont end up saving the config, this will avoid unnecessary IO operations

namespace Infinity {

    struct AppConfig {
        std::string cache_path;
        std::string key_location;
        bool lua_release;

        AppConfig();
    };

    class Config {

    public:
        static Config &getInstance();
        void SaveConfig() const;
        AppConfig GetConfig();
        void UpdateConfig(const AppConfig &config);
        ~Config();

    private:
        static Config *m_Instance;
        std::filesystem::path m_ConfigPath;
        AppConfig m_AppConfig;

        Config(const Config &) = delete;
        Config &operator=(const Config &) = delete;

        void LoadConfig();

        static std::string GetFilePath();

        Config();
    };


} // namespace Infinity
