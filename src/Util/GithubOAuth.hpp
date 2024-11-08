
#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "Encrypt.hpp"
#include "curl/curl.h"
#include "imgui.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class GithubOAuth {
public:
    GithubOAuth();

    void RenderUI(const std::function<void(const std::string &)> &key_set_callback);

    std::optional<std::string> GetOAuthToken();
    [[nodiscard]] bool IsAuthenticated() const { return m_IsAuthorized; }
    [[nodiscard]] std::string GetUsername() const { return m_Username; }
    [[nodiscord]] static std::optional<GithubOAuth *> GetInstance() {
        if (s_Instance != nullptr) {
            return s_Instance;
        }
        return std::nullopt;
    }

private:
    bool StartDeviceFlow();
    void PollForAccessToken();
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    bool VerifyAccessToken();


private:
    std::string m_DeviceCode;
    int m_Interval;

    const std::string m_ClientID = "Ov23liiC9KSl2GTqcfLj";
    std::string m_AccessToken;
    bool m_IsAuthorized = false;
    std::string m_Username;
    static GithubOAuth *s_Instance;
};
