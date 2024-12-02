

#include "Notifications.hpp"
#include <filesystem>
#include <iostream>

namespace Infinity {

    std::wstring Notification::GetFilePath() {

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
        return output_path.wstring();
    }

#ifdef WIN32

    void Notification::SendNotification(const NotificationContent &notification_content) {

        auto m_WinToast = WinToastTemplate(WinToastTemplate::ImageAndText02);
        if (!WinToast::isCompatible()) {
            std::wcerr << L"Error, your system in not supported!" << std::endl;
        }
        auto image_path = Notification::GetFilePath() + L"/icons";
        m_WinToast.setTextField(std::filesystem::path(notification_content.summary).wstring(), WinToastTemplate::FirstLine);
        m_WinToast.setTextField(std::filesystem::path(notification_content.body).wstring(), WinToastTemplate::SecondLine);
        m_WinToast.setImagePath(notification_content.notification_type == NotificationType::Error ? image_path + L"/error.png" : image_path + L"/status.png");
        WinToast::instance()->setAppName(L"InfinityPackageBuilder");
        WinToast::instance()->setAppUserModelId(L"Infinity Package Builder");

        if (!WinToast::instance()->initialize()) {
            std::wcerr << L"Error, your system in not compatible!" << std::endl;
        }

        WinToast::instance()->showToast(m_WinToast, new CustomHandler());
    }

#else

    void Notification::SendNotification(const NotificationContent &notification_content) {
        auto notification = MiniKdeNotify::Notification::Builder("InfinityPackageBuilder")
                                    .summary(notification_content.summary)
                                    .body(notification_content.body)
                                    .icon(notification_content.notification_type == NotificationType::Error ? "dialog-warning" : "dialog-information")
                                    .timeout(10000)
                                    .build();
        notification->send();
    }

#endif
} // namespace Infinity
