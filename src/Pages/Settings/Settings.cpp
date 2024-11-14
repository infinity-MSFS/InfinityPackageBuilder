
#include "Settings.hpp"

#include "Util/OpenInBrowser.hpp"
#include "renderer/GUI/ApplicationGui.hpp"
#include "Util/Notifications.hpp"


Settings::Settings(const float padding_x, const float padding_y) :
        Page(padding_x, padding_y) {}

void Settings::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Settings");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
    ImGui::Text("Settings");
    if (ImGui::Button("Show Toast")) {
        InfinityPackageBuilder::Util::Notifications::Notification::SendNotification({"Hello", "A PSA from the settings",
                                                                                     InfinityPackageBuilder::Util::Notifications::NotificationType::Status});
    }
    if (ImGui::Button("Open in browser")) {
        std::string url = "https://youtube.com";
        OpenUrlInBrowser(url);
    }
}
