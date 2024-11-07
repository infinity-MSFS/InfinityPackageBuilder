
#include "GithubRelease.hpp"
#include <iostream>


void GithubRelease::RenderUI() {
    ImGui::Begin("GitHub Release Creator");

    // ImGui::InputText("Repository (username/repo)", &m_Repo);
    // ImGui::InputText("Release Title", &releaseTitle);
    // ImGui::InputText("Release Description", &releaseDescription);
    // ImGui::InputText("Tag (e.g., v1.0.0)", &releaseTag);
    // ImGui::InputText("File Path (e.g., /path/to/file.zip)", &filePath);
    // ImGui::Checkbox("Is Prerelease", &isPrerelease);
    // ImGui::InputText("GitHub Token", &githubToken);
    //
    // if (ImGui::Button("Publish Release")) {
    //     if (CreateRelease()) {
    //         std::cout << "Release created successfully.\n";
    //     } else {
    //         std::cerr << "Failed to create release.\n";
    //     }
    // }

    ImGui::End();
}


bool GithubRelease::CreateRelease() {
    if (!CreateTag()) {
        std::cerr << "Failed to create tag on GitHub.\n";
        return false;
    }

    // Step 2: Use GitHub API to create release
    std::string releaseId;
    if (!CreateGithubRelease(releaseId)) {
        std::cerr << "Failed to create release on GitHub.\n";
        return false;
    }

    // Step 3: Upload file if specified
    if (!m_FilePath.empty() && !releaseId.empty()) {
        return UploadAsset(releaseId);
    }

    return true;
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

        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: token " + githubToken).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/octet-stream");

        std::string url = "https://uploads.github.com/repos/" + m_Repo + "/releases/" + releaseId + "/assets?name=" + GetFileName(m_FilePath);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fileContents.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fileContents.size());

        // Perform the request
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
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: token " + githubToken).c_str());
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
