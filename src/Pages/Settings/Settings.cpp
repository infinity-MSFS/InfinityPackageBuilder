
#include "Settings.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


Settings::Settings(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void Settings::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Settings");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
    ImGui::Text("Settings");
}
