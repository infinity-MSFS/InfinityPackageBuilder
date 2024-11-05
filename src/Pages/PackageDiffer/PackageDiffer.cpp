
#include "PackageDiffer.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


PackageDiffer::PackageDiffer(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void PackageDiffer::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Package Differ");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, ImGui::GetWindowHeight() / 2));
    ImGui::Text("Package Differ");
}
