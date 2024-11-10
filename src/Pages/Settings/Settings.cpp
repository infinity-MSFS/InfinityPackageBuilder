
#include "Settings.hpp"

#include "Util/OpenInBrowser.hpp"
#include "renderer/GUI/ApplicationGui.hpp"

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

Settings::Settings(const float padding_x, const float padding_y) : Page(padding_x, padding_y), m_WinToast(WinToastTemplate::ImageAndText02) {
    if (!WinToast::isCompatible()) {
        std::wcerr << L"Error, your system in not supported!" << std::endl;
    }
    m_WinToast.setTextField(L"Monkeys will rise", WinToastTemplate::FirstLine);
    m_WinToast.setTextField(L"THE MONEYS ARE COMING!", WinToastTemplate::SecondLine);
    m_WinToast.setHeroImagePath(L"C:/Users/Taco/Downloads/gud.png");

    WinToast::instance()->setAppName(L"Settings");
    WinToast::instance()->setAppUserModelId(L"Infinity Package Builder");

    if (!WinToast::instance()->initialize()) {
        std::wcerr << L"Error, your system in not compatible!" << std::endl;
    }
}

void Settings::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Settings");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
    ImGui::Text("Settings");
    if (ImGui::Button("Show Toast")) {
        WinToast::instance()->showToast(m_WinToast, new CustomHandler());
    }
    if (ImGui::Button("Open in browser")) {
        std::string url = "https://youtube.com";
        OpenUrlInBrowser(url);
    }
}
