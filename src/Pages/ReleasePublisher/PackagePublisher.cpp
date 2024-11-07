
#include "PackagePublisher.hpp"

#include "Components/ToggleButton/ToggleButton.hpp"
#include "Util/LuaInterpreter.hpp"
#include "renderer/GUI/ApplicationGui.hpp"

#include "Util/FunctionTraits.hpp"
#include "Util/GithubOAuth.hpp"
#include "sol/sol.hpp"

void greet(const std::string &name) { std::cout << "Hello " << name << "!" << std::endl; }

void windowSize() { std::cout << "Height: " << ImGui::GetWindowHeight() << "px, Width: " << ImGui::GetWindowWidth() << "px" << std::endl; }


PackagePublisher::PackagePublisher(const float padding_x, const float padding_y, const char *lua_file_path) : Page(padding_x, padding_y), m_LuaInterpreter(lua_file_path) {

    std::string greet_signature = get_function_signature<void, std::string>("greet");
    m_LuaInterpreter.AddFunctionToLua("greet", greet, greet_signature);
    std::string window_size_signature = get_function_signature<void, void>("windowSize");
    m_LuaInterpreter.AddFunctionToLua("windowSize", windowSize, window_size_signature);
}


using namespace InfinityPackageBuilder::Lua;
void PackagePublisher::RenderPage() {
    const auto word_size = ImGui::CalcTextSize("Package Publisher");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
    ImGui::Text("Package Publisher");


    static bool is_using_lua = false;

    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 820.0f, 50.0f));
    ImGui::Text("Lua Mode");
    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 700.0f, 50.0f));
    ToggleButton("lua", &is_using_lua);

    if (is_using_lua) {
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 600.0f, 50.0f));
        m_LuaInterpreter.RenderCodebox();
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 820.0f, 100.0f));

        const char *button_label;

        if (m_LuaInterpreter.GetLuaError().line_number != -1) {
            button_label = "Try Again";
        } else {
            button_label = "Run";
        }

        if (ImGui::Button(button_label)) {
            m_LuaInterpreter.RunLua();
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 820.0f, 150.0f));
        if (ImGui::Button("Paste from Clipboard")) {
            m_LuaInterpreter.Paste();
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() - 820.0f, 200.0f));
        if (ImGui::Button("Save Script")) {
            m_LuaInterpreter.Save();
        }
    }
}
