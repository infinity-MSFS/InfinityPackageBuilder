
#include "PackageBuilder.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


PackageBuilder::PackageBuilder(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

#ifndef WIN32
void PackageBuilder::RenderPage() { ImGui::Text("PackageBuilder"); }
#else
void PackageBuilder::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Package builder is only supported on windows");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, ImGui::GetWindowHeight() / 2));
    ImGui::Text("Package builder is only supported on windows");
}
#endif
