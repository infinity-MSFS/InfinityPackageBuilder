
#include "LauncherJsonManager.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


LauncherJsonManager::LauncherJsonManager(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void LauncherJsonManager::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Launcher Json Manager");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
    ImGui::Text("Launcher Json Manager");
}
