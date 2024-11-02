
#include "LauncherJsonManager.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


LauncherJsonManager::LauncherJsonManager(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void LauncherJsonManager::RenderPage() { ImGui::Text("LauncherJsonManager"); }
