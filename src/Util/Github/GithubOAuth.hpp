
#pragma once

#include <chrono>
#include <iostream>
#include <optional>
#include <thread>

#include "Json/json.hpp"
#include "Util/Encryption/Encryption.hpp"
#include "curl/curl.h"
#include "imgui.h"

namespace Infinity {
    using json = nlohmann::json;

    enum class GithubOuthState {
        UNAUTHORIZED,
        PENDING_AUTHORIZATION,
        AUTHORIZED,
    };


    class GithubOAuth {
    public:
        GithubOAuth();

        void RenderUI(const std::function<void(const std::string &)> &key_set_callback);

        std::optional<std::string> GetOAuthToken();
        [[nodiscard]] bool IsAuthenticated() const { return m_IsAuthorized; }
        [[nodiscard]] std::string GetUsername() const { return m_Username; }
        [[nodiscard]] static std::optional<GithubOAuth *> GetInstance() {
            if (s_Instance != nullptr) {
                return s_Instance;
            }
            return std::nullopt;
        }
        [[nodiscard]] std::string GetDeviceCode() const { return m_DeviceCode; }

    private:
        bool StartDeviceFlow();
        void PollForAccessToken();
        static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
        bool VerifyAccessToken();


    private:
        std::string m_DeviceCode;
        std::string m_UserCode;
        int m_Interval;

        const std::string m_ClientID = "Ov23liiC9KSl2GTqcfLj";
        std::string m_AccessToken;
        bool m_IsAuthorized = false;
        std::string m_Username;
        static GithubOAuth *s_Instance;
    };
} // namespace Infinity
