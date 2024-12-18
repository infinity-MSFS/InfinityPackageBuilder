
#include "Menu.hpp"
#include <iostream>

#include "Frontend/Components/Buttons/MenuButton.hpp"
#include "Util/Errors/Error.hpp"
#include "Util/Router/Router.hpp"

namespace Infinity {
    MenuManager::State MenuManager::m_CurrentState = State::Closed;


    bool MenuUI::RenderSettingsIcon(const ImVec2 pos, const MenuManager::State state) {
        SettingsIconAnimationParams settings_icon_animation_params;
        settings_icon_animation_params.top = LineParams{.start = ImVec2(pos.x + 10.0f, pos.y + 10.0f), .end = ImVec2(pos.x + 40.0f, pos.y + 10.0f)},
        settings_icon_animation_params.middle = LineParams{.start = ImVec2(pos.x + 10.0f, pos.y + 22.0f), .end = ImVec2(pos.x + 40.0f, pos.y + 22.0f)},
        settings_icon_animation_params.bottom = LineParams{.start = ImVec2(pos.x + 10.0f, pos.y + 34.0f), .end = ImVec2(pos.x + 40.0f, pos.y + 34.0f)};

        if (state == MenuManager::State::Open) {
            settings_icon_animation_params.top.start = ImVec2(pos.x + 10.0f, pos.y + 34.0f);
            settings_icon_animation_params.bottom.start = ImVec2(pos.x + 10.0f, pos.y + 10.0f);
        }

        auto *draw_list = ImGui::GetWindowDrawList();

        draw_list->AddLine(settings_icon_animation_params.top.start, settings_icon_animation_params.top.end, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
        if (state == MenuManager::State::Closed) {
            ImGui::GetWindowDrawList()->AddLine(settings_icon_animation_params.middle.start, settings_icon_animation_params.middle.end, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
        }
        ImGui::GetWindowDrawList()->AddLine(settings_icon_animation_params.bottom.start, settings_icon_animation_params.bottom.end, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
        ImGui::SetCursorPos(ImVec2(pos.x, pos.y));

        const bool clicked = ImGui::InvisibleButton("SettingsMenu", ImVec2(50.0f, 50.0f));

        return clicked;
    }


    static float settings_menu_x = -10.0f;
    static constexpr float animation_speed = 6.0f;


    static MenuButton menu_button(settings_menu_x - 300.0f, 130.0f, 300.0f, 50.0f, 20.0f, buttons);

    float MenuUI::RenderSettingsMenu(const MenuManager::State state) {


        if (const float target_x = (state == MenuManager::State::Open) ? 405.0f : -10.0f; settings_menu_x != target_x) {

            if (const float delta = (target_x - settings_menu_x) * animation_speed * ImGui::GetIO().DeltaTime; std::abs(delta) < 0.1f) {
                settings_menu_x = target_x;
            } else {
                settings_menu_x += delta;
            }
        }
#ifdef WIN32
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(-10.0f, 40.0f), ImVec2(settings_menu_x, ImGui::GetWindowHeight()), ImColor(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 0.55f), 0.0f);
        const auto text_size = ImGui::CalcTextSize("Tools").x / 2;
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 202.5f - text_size, 60.0f));
        ImGui::Text("Tools");
        ImGui::GetWindowDrawList()->AddLine(ImVec2(settings_menu_x - 395.0f, 100.0f), ImVec2(settings_menu_x - 10.0f, 100.0f), ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
#else
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(-10.0f, 0.0f), ImVec2(settings_menu_x, ImGui::GetWindowHeight()), ImColor(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 0.75f), 0.0f);
        const auto text_size = ImGui::CalcTextSize("Tools").x / 2;
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 202.5f - text_size, 20.0f));
        ImGui::Text("Tools");
        ImGui::GetWindowDrawList()->AddLine(ImVec2(settings_menu_x - 395.0f, 60.0f), ImVec2(settings_menu_x - 10.0f, 60.0f), ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);
#endif


        menu_button.UpdateLocation(settings_menu_x - 352.5f, 130.0f);
        menu_button.Render();

        return settings_menu_x;
    }
} // namespace Infinity
