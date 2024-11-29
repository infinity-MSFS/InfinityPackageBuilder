
#include "GithubRelease.hpp"
#include <iostream>
#include <thread>

namespace Infinity {

    void GithubRelease::RenderUI() {
        ImGui::Begin("GitHub Release Creator");

        char repoBuffer[256] = {0};
        char releaseTitleBuffer[256] = {0};
        char releaseDescriptionBuffer[512] = {0};
        char releaseTagBuffer[256] = {0};
        char filePathBuffer[512] = {0};

        strncpy(repoBuffer, m_Repo.c_str(), sizeof(repoBuffer) - 1);
        strncpy(releaseTitleBuffer, m_ReleaseTitle.c_str(), sizeof(releaseTitleBuffer) - 1);
        strncpy(releaseDescriptionBuffer, m_ReleaseDescription.c_str(), sizeof(releaseDescriptionBuffer) - 1);
        strncpy(releaseTagBuffer, m_ReleaseTag.c_str(), sizeof(releaseTagBuffer) - 1);
        strncpy(filePathBuffer, m_FilePath.c_str(), sizeof(filePathBuffer) - 1);

        if (ImGui::InputText("Repository (username/repo)", repoBuffer, sizeof(repoBuffer))) {
            m_Repo = repoBuffer;
        }

        if (ImGui::InputText("Release Title", releaseTitleBuffer, sizeof(releaseTitleBuffer))) {
            m_ReleaseTitle = releaseTitleBuffer;
        }

        if (ImGui::InputText("Release Description", releaseDescriptionBuffer, sizeof(releaseDescriptionBuffer))) {
            m_ReleaseDescription = releaseDescriptionBuffer; // Update the member variable after input
        }

        if (ImGui::InputText("Tag (e.g., v1.0.0)", releaseTagBuffer, sizeof(releaseTagBuffer))) {
            m_ReleaseTag = releaseTagBuffer; // Update the member variable after input
        }

        if (ImGui::InputText("File Path (e.g., /path/to/file.zip)", filePathBuffer, sizeof(filePathBuffer))) {
            m_FilePath = filePathBuffer; // Update the member variable after input
        }
        ImGui::Checkbox("Is Prerelease", &m_IsPrerelease);


        if (m_Loading == LoadingState::Loading) {
            ImGui::Text("Creating Release....");
            ImGui::ProgressBar(0.0f, {0.0f, 0.0f}, "Please wait...");
        }
        if (m_Loading == LoadingState::PreLoad) {
            if (ImGui::Button("Publish Release")) {
                std::thread([this]() {
                    std::function<void(const LoadingState)> callback = [&](const LoadingState loading) { SetLoading(loading); };
                    CreateRelease(callback);
                }).detach();
            }
        }
        if (m_Loading == LoadingState::AfterLoad) {
            ImGui::Text("Package Published");
            m_Timer.Update();
            if (!m_Timer.IsActive()) {
                SetLoading(LoadingState::PreLoad);
            }
        }

        if (m_Loading == LoadingState::Failure) {
            ImGui::Text("Failed to upload release");
            m_Timer.Update();
            if (!m_Timer.IsActive()) {
                SetLoading(LoadingState::PreLoad);
            }
        }


        ImGui::End();
    }


    bool GithubRelease::CreateRelease(const std::function<void(const LoadingState)> &callback) {
        callback(LoadingState::Loading);
        if (!CreateTag()) {
            callback(LoadingState::Failure);
            std::cerr << "Failed to create tag on GitHub.\n";
            return false;
        }

        std::string releaseId;
        if (!CreateGithubRelease(releaseId)) {
            std::cerr << "Failed to create release on GitHub.\n";
            callback(LoadingState::Failure);
            return false;
        }

        if (!m_FilePath.empty() && !releaseId.empty()) {
            if (UploadAsset(releaseId)) {

                callback(LoadingState::AfterLoad);
                return true;
            } else {
                return false;
            }
        }

        return false;
    }

    bool GithubRelease::CreateTag() {
        json payload = {{"tag", m_ReleaseTag}, {"message", "Release " + m_ReleaseTag}, {"object", "main"}, {"type", "commit"}};

        std::string url = "/repos/" + m_Repo + "/git/tags";
        return SendGitHubRequest(url, payload);
    }


    bool GithubRelease::CreateGithubRelease(std::string &release_id) {
        json payload = {{"tag_name", m_ReleaseTag}, {"name", m_ReleaseTitle}, {"body", m_ReleaseDescription}, {"prerelease", m_IsPrerelease}};
        std::string url = "/repos/" + m_Repo + "/releases";
        std::string response;
        if (!SendGitHubRequest(url, payload, &response)) {
            return false;
        }
        if (auto response_json = json::parse(response); response_json.contains("id")) {
            release_id = std::to_string(response_json["id"].get<int>());
            return true;
        }
        return false;
    }

    bool GithubRelease::UploadAsset(const std::string &releaseId) const {
        CURL *curl;
        CURLcode res;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            // Prepare the file
            std::ifstream file(m_FilePath, std::ios::binary);
            std::stringstream buffer;
            buffer << file.rdbuf();
            std::string fileContents = buffer.str();

            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, ("Authorization: token " + m_GithubToken).c_str());
            headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

            std::string url = "https://uploads.github.com/repos/" + std::string(m_Repo) + "/releases/" + releaseId + "/assets?name=" + GetFileName(m_FilePath);

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fileContents.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fileContents.size());

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "Failed to upload file: " << curl_easy_strerror(res) << '\n';
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return false;
            }

            std::cout << "File uploaded successfully.\n";

            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return true;
        }

        std::cerr << "Curl initialization failed.\n";
        curl_global_cleanup();
        return false;
    }

    bool GithubRelease::SendGitHubRequest(const std::string &url, const json &payload, std::string *response) const {
        CURL *curl;
        CURLcode res;
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if (curl) {
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, ("Authorization: token " + m_GithubToken).c_str());
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "User-Agent: C++ GitHub API Client");

            std::string payloadStr = payload.dump();

            curl_easy_setopt(curl, CURLOPT_URL, ("https://api.github.com" + url).c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payloadStr.c_str());

            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                if (response) {
                    *response = readBuffer;
                }
                curl_easy_cleanup(curl);
                curl_global_cleanup();
                return true;
            }

            std::cerr << "Failed to send request: " << curl_easy_strerror(res) << '\n';
            curl_easy_cleanup(curl);
        }

        curl_global_cleanup();
        return false;
    }

    size_t GithubRelease::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userData) {
        size_t totalSize = size * nmemb;
        userData->append((char *) contents, totalSize);
        return totalSize;
    }


    std::string GithubRelease::GetFileName(const std::string &path) {
        size_t pos = path.find_last_of("/\\");
        return (pos == std::string::npos) ? path : path.substr(pos + 1);
    }
    void GithubRelease::SetKey(const std::string &key) { m_GithubToken = key; }
} // namespace Infinity
