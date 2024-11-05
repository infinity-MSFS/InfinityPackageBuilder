
#include "PackagePublisher.hpp"

#include "Util/LuaInterpreter.hpp"
#include "renderer/GUI/ApplicationGui.hpp"

#include "sol/sol.hpp"
#include "util/FunctionTraits.hpp"

void greet(const std::string &name) { std::cout << "Hello " << name << "!" << std::endl; }


PackagePublisher::PackagePublisher(const float padding_x, const float padding_y, const char *lua_file_path) : Page(padding_x, padding_y), m_LuaInterpreter(lua_file_path) {

    std::string greet_signature = get_function_signature<void, std::string>("greet");
    m_LuaInterpreter.AddFunctionToLua("greet", greet, greet_signature);
}
using namespace InfinityPackageBuilder::Lua;
void PackagePublisher::RenderPage() {


    ImGui::SetCursorPos(ImVec2(300.0f, 100.0f));
    m_LuaInterpreter.RenderCodebox();
    ImGui::SetCursorPos(ImVec2(120.0f, 100.0f));

    const char *button_label;

    if (m_LuaInterpreter.GetLuaError().line_number != -1) {
        button_label = "Error Running, Try again?";
    } else {
        button_label = "Run";
    }

    if (ImGui::Button(button_label)) {
        m_LuaInterpreter.RunLua();
    }

    ImGui::SetCursorPos(ImVec2(120.0f, 150.0f));
    if (ImGui::Button("Paste from Clipboard")) {
        m_LuaInterpreter.Paste();
    }
}
