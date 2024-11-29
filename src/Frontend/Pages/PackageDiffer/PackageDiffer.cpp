
#include "PackageDiffer.hpp"

#include "Backend/Application/Application.hpp"
#include "Util/Github/GithubOAuth.hpp"

namespace Infinity {
    PackageDiffer::PackageDiffer(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}


    void PackageDiffer::RenderPage() {
        const auto word_size = ImGui::CalcTextSize("Package Differ");
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
        ImGui::Text("Package Differ");
    }
} // namespace Infinity
