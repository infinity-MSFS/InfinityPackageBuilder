
#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>


#include "curl/curl.h"
#include "imgui.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

class GithubOAuth {
public:
    GithubOAuth();

    void RenderUI();

    std::optional<std::string> GetOAuthToken();
    [[nodiscard]] bool IsAuthenticated() const { return m_IsAuthorized; }
    [[nodiscard]] std::string GetUsername() const { return m_Username; }

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
};
