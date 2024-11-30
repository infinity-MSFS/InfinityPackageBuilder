#pragma once

#ifdef WIN32
#include "wintoastlib.h"
#else

#include "MiniKDENotify/MiniKDENotify.hpp"

#endif

#include <string>

namespace Infinity {
    enum class NotificationType { Error, Status };

    struct NotificationContent {
        std::string summary;
        std::string body;
        NotificationType notification_type;
    };
#ifdef WIN32
    using namespace WinToastLib;

    class CustomHandler : public IWinToastHandler {
    public:
        void toastActivated() const {
            std::wcout << L"The user clicked in this toast" << std::endl;
            exit(0);
        }

        void toastActivated(int actionIndex) const {
            std::wcout << L"The user clicked on action #" << actionIndex << std::endl;
            exit(16 + actionIndex);
        }

        void toastDismissed(WinToastDismissalReason state) const {
            switch (state) {
                case UserCanceled:
                    std::wcout << L"The user dismissed this toast" << std::endl;
                    break;
                case TimedOut:
                    std::wcout << L"The toast has timed out" << std::endl;
                    break;
                case ApplicationHidden:
                    std::wcout << L"The application hid the toast using ToastNotifier.hide()" << std::endl;
                    break;
                default:
                    std::wcout << L"Toast not activated" << std::endl;
                    break;
            }
        }

        void toastFailed() const {
            std::wcout << L"Error showing current toast" << std::endl;
            exit(5);
        }
    };


    class Notification {
        public:
        static void SendNotification(const NotificationContent &notification_content);
    };

#else

    class Notification {
    public:
        static void SendNotification(const NotificationContent &notification_content);
    };

#endif
} // namespace Infinity
