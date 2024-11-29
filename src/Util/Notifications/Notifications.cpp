

#include "Notifications.hpp"
#include <filesystem>
#include <iostream>

namespace Infinity {

#ifdef WIN32

    void Notification::SendNotification(const Notifications::NotificationContent &notification_content) {

        auto m_WinToast = WinToastTemplate(WinToastTemplate::ImageAndText02);
        if (!WinToast::isCompatible()) {
            std::wcerr << L"Error, your system in not supported!" << std::endl;
        }
        m_WinToast.setTextField(std::filesystem::path(notification_content.summary).wstring(), WinToastTemplate::FirstLine);
        m_WinToast.setTextField(std::filesystem::path(notification_content.body).wstring(), WinToastTemplate::SecondLine);
        m_WinToast.setHeroImagePath(notification_content.notification_type == NotificationType::Error ? L"C:\\Windows\\System32\\shell32.dll,-79" : L"C:\\Windows\\System32\\shell32.dll,-221");

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
