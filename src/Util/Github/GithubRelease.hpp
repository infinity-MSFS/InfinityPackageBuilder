
#pragma once


#include <fstream>
#include <sstream>
#include <string>

#include "Json/json.hpp"
#include "Util/StateTimer.hpp"
#include "curl/curl.h"
#include "imgui.h"

namespace Infinity {
    using json = nlohmann::json;

    enum class LoadingState {
        PreLoad,
        Loading,
        AfterLoad,
        Failure,
    };

    class GithubRelease {


    public:
        void RenderUI();

        void SetKey(const std::string &key);

    private:
        bool CreateRelease(const std::function<void(const LoadingState)> &callback);
        bool CreateTag();
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData);

        bool CreateGithubRelease(std::string &release_id);
        bool UploadAsset(const std::string &releaseId) const;
        bool SendGitHubRequest(const std::string &url, const json &payload, std::string *response = nullptr) const;
        static std::string GetFileName(const std::string &path);

        void SetLoading(const LoadingState state) {
            m_Loading = state;
            if (state == LoadingState::AfterLoad || state == LoadingState::Failure) {
                m_Timer.Start(5);
            }
        }

    private:
        std::string m_Repo;
        std::string m_ReleaseTitle;
        std::string m_ReleaseDescription;
        std::string m_ReleaseTag;
        std::string m_FilePath;
        LoadingState m_Loading = LoadingState::PreLoad;
        bool m_IsPrerelease;
        std::string m_GithubToken;
        StateTimer m_Timer;
    };
} // namespace Infinity
