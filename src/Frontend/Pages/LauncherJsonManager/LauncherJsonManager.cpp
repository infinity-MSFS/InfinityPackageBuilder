
#include "LauncherJsonManager.hpp"

#include "Backend/Application/Application.hpp"

namespace Infinity {
    LauncherJsonManager::LauncherJsonManager(const float padding_x, const float padding_y) : Page(padding_x, padding_y), m_FileSelector() {}

    void LauncherJsonManager::RenderPage() {
        const auto word_size = ImGui::CalcTextSize("Launcher Json Manager");
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
        ImGui::Text("Launcher Json Manager");


        m_FileSelector.Render();
    }
} // namespace Infinity
