
#include "PackagePublisher.hpp"

#include "renderer/GUI/ApplicationGui.hpp"


PackagePublisher::PackagePublisher(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void PackagePublisher::RenderPage() { ImGui::Text("PackagePublisher"); }
