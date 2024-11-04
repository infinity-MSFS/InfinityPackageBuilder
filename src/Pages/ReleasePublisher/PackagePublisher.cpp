
#include "PackagePublisher.hpp"

#include "renderer/GUI/ApplicationGui.hpp"

#include "sol/sol.hpp"


PackagePublisher::PackagePublisher(const float padding_x, const float padding_y) : Page(padding_x, padding_y) {}

void PackagePublisher::RenderPage() {

    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math, sol::lib::io, sol::lib::os,
                       sol::lib::debug);

    static char tex[4096] = "";

    ImGui::SetCursorPos(ImVec2(300.0f, 100.0f));

    ImGui::InputTextMultiline("Lua Script", tex, sizeof(tex), ImVec2(400.0f, 600.0f));
    ImGui::SetCursorPos(ImVec2(920.0f, 100.0f));
    if (ImGui::Button("Exec")) {
        try {
            lua.script(tex);
            // lua.script_file("C://test//script.lua");
        } catch (const sol::error &e) {
            std::cerr << "Lua error:" << e.what() << std::endl;
        }
    }
    ImGui::SetCursorPos(ImVec2(920.0f, 150.0f));
    if (ImGui::Button("Paste from Clipboard")) {
        const char *clipboard_text = ImGui::GetClipboardText();
        strncpy_s(tex, clipboard_text, sizeof(tex) - 1);
        tex[sizeof(tex) - 1] = '\0';
    }

    ImGui::Text("PackagePublisher");
}
