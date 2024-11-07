
#pragma once


#include <fstream>
#include <sstream>
#include <string>

#include "curl/curl.h"
#include "imgui.h"
#include "nlohmann/json.hpp"


using json = nlohmann::json;

class GithubRelease {

public:
    std::string m_Repo;
    std::string m_ReleaseTitle;
    std::string m_ReleaseDescription;
    std::string m_ReleaseTag;
    std::string m_FilePath;
    bool m_IsPrerelease;
    std::string githubToken;

    void RenderUI();

private:
    bool CreateRelease();
    bool CreateTag();
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData);

    bool CreateGithubRelease(std::string &release_id);
    bool UploadAsset(const std::string &releaseId) const;
    bool SendGitHubRequest(const std::string &url, const json &payload, std::string *response = nullptr) const;
    static std::string GetFileName(const std::string &path);
};
