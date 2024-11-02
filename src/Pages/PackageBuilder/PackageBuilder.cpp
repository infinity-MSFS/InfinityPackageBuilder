
#include "PackageBuilder.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


PackageBuilder::PackageBuilder(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void PackageBuilder::RenderPage() { ImGui::Text("PackageBuilder"); }
