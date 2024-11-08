
#include "GithubOAuth.hpp"
#include <chrono>
#include <ini/ini.h>

constexpr const char *encryption_key = ENCRYPTION_KEY;

GithubOAuth *GithubOAuth::s_Instance = nullptr;

GithubOAuth::GithubOAuth() : m_Interval(0) {
    mINI::INIFile file(".infinity.ini");
    mINI::INIStructure ini;
    s_Instance = this;
    file.read(ini);
    if (ini.has("github")) {
        if (auto &category = ini["github"]; category.has("token")) {
            m_AccessToken = Decrypt(category["token"], encryption_key);
            VerifyAccessToken();
        }
    }
}

void GithubOAuth::RenderUI(const std::function<void(const std::string &)> &key_set_callback) {

    ImGui::SetCursorPos({ImGui::GetWindowWidth() / 2.0f, ImGui::GetWindowHeight() / 2.0f});
    if (!m_IsAuthorized) {
        if (ImGui::Button("Authorize with GitHub")) {
            if (StartDeviceFlow()) {
                std::cout << "Follow the instructions to authorize the app.\n";
            } else {
                std::cerr << "Failed to start device flow.\n";
            }
        }
    } else {
        key_set_callback(m_AccessToken);
        ImGui::Text("Welcome %s", m_Username.c_str());
    }
}

std::unordered_map<std::string, std::string> parse_urlencoded(const std::string &str) {
    std::unordered_map<std::string, std::string> result;
    std::istringstream ss(str);
    std::string item;

    while (std::getline(ss, item, '&')) {
        std::string key, value;
        std::istringstream key_value(item);
        if (std::getline(std::getline(key_value, key, '='), value)) {
            std::string decoded_key = curl_easy_unescape(nullptr, key.c_str(), key.length(), nullptr);
            std::string decoded_value = curl_easy_unescape(nullptr, value.c_str(), value.length(), nullptr);
            result[decoded_key] = decoded_value;
        }
    }
    return result;
}

bool GithubOAuth::StartDeviceFlow() {
    std::cout << "Starting device flow.\n";

    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl.\n";
        return false;
    }

    std::cout << "Curl initialized.\n";

    std::string url = "https://github.com/login/device/code";
    json payload = {{"client_id", m_ClientID}, {"scope", "repo"}};
    std::string body = payload.dump();

    std::cout << "Body: " << body << "\n";

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());

    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
    }
    curl_slist_free_all(headers);

    std::cout << response_string << "\n";
    std::unordered_map<std::string, std::string> response_map = parse_urlencoded(response_string);

    json response_json;
    for (const auto &pair: response_map) {
        std::cout << pair.first << ": " << pair.second << "\n";
        response_json[pair.first] = pair.second;
    }
    m_DeviceCode = response_json["device_code"];
    std::string userCode = response_json["user_code"];
    std::string verificationUri = response_json["verification_uri"];
    std::string interval_string = response_json["interval"];
    m_Interval = std::stoi(interval_string);

    std::cout << "Go to " << verificationUri << " and enter the code: " << userCode << "\n";

    std::thread(&GithubOAuth::PollForAccessToken, this).detach();
    curl_easy_cleanup(curl);
    return true;
}

std::optional<std::string> GithubOAuth::GetOAuthToken() {
    if (m_IsAuthorized) {
        return m_AccessToken;
    } else {
        return std::nullopt;
    }
}


void GithubOAuth::PollForAccessToken() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl.\n";
        return;
    }

    std::string url = "https://github.com/login/oauth/access_token";

    while (!m_IsAuthorized) {
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<long>(m_Interval + 1)));
        json payload = {{"client_id", m_ClientID}, {"device_code", m_DeviceCode}, {"grant_type", "urn:ietf:params:oauth:grant-type:device_code"}};
        std::string body = payload.dump();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        std::string response_string;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            break;
        }
        curl_slist_free_all(headers);
        std::unordered_map<std::string, std::string> response_map = parse_urlencoded(response_string);

        std::cout << response_string << "\n";

        json response_json;
        for (const auto &pair: response_map) {
            std::cout << pair.first << ": " << pair.second << "\n";
            response_json[pair.first] = pair.second;
        }

        if (response_string.contains("access_token")) {
            m_AccessToken = response_json["access_token"];
            m_IsAuthorized = true;
            VerifyAccessToken();
            mINI::INIFile file(".infinity.ini");

            mINI::INIStructure ini;
            ini["github"]["token"] = Encrypt(m_AccessToken, encryption_key);

            if (!file.generate(ini, true)) {
                std::cerr << "Failed to generate INI file.\n";
            }
#ifdef WIN32
            if (!HideFile(.infinity.ini)) {
                std::cerr << "Failed to hide INI file\n";
            }
#endif
            break;
        } else if (response_string.contains("interval")) {
            std::string interval_string = response_json["interval"];
            m_Interval = std::stoi(interval_string);
        } else if (response_string.contains("error") && response_json["error"] == "authorization_pending") {
            std::this_thread::sleep_for(std::chrono::seconds(static_cast<long>(m_Interval + 1)));
        } else {
            std::cerr << "Authorization failed: " << response_json.dump() << '\n';
            break;
        }
    }

    curl_easy_cleanup(curl);
}

bool GithubOAuth::VerifyAccessToken() {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl.\n";
        return false;
    }

    std::string url = "https://api.github.com/user";

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, ("Authorization: token " + m_AccessToken).c_str());
    headers = curl_slist_append(headers, "User-Agent: InfinityPackageBuilder");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    std::string response_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return false;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    try {
        json response_json = json::parse(response_string);
        if (response_json.contains("login")) {
            m_Username = response_json["login"];
            m_IsAuthorized = true;
            return true;
        } else {
            std::cerr << "Error: Unable to fetch username from response: " << response_string << std::endl;
            return false;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error parsing response: " << e.what() << std::endl;
        return false;
    }
}


size_t GithubOAuth::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t total_size = size * nmemb;
    ((std::string *) userp)->append((char *) contents, total_size);
    return total_size;
}
