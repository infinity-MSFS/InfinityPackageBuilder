
#include "Settings.hpp"

#include "Backend/Application/Application.hpp"
#include "Util/Notifications/Notifications.hpp"
#include "Util/OpenInBrowser.hpp"
#include "zoe/zoe.h"

namespace Infinity {
    Settings::Settings(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}


    void Settings::RenderPage() {
        const auto word_size = ImGui::CalcTextSize("Settings");
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
        ImGui::Text("Settings");
        if (ImGui::Button("Show Toast")) {
            Notification::SendNotification({"Summary", "Body", NotificationType::Status});
        }
        if (ImGui::Button("Open in browser")) {
            std::string url = "https://youtube.com";
            OpenUrlInBrowser(url);
        }
    }
} // namespace Infinity
