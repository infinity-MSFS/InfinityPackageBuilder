
#include "PackageDiffer.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


PackageDiffer::PackageDiffer(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void PackageDiffer::RenderPage() { ImGui::Text("PackageDiffer"); }
