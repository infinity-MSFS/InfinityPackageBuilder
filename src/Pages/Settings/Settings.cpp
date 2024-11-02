
#include "Settings.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


Settings::Settings(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void Settings::RenderPage() { ImGui::Text("Settings"); }
