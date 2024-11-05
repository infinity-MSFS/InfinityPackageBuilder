
#include "Menu.hpp"
#include <iostream>
#include "Router/Router.hpp"
#include "Util/Error.hpp"

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

using InfinityPackageBuilder::Utils::Router;

float MenuUI::RenderSettingsMenu(const MenuManager::State state) {


    if (const auto router = Router::getInstance(); router.has_value()) {
        if (const float target_x = (state == MenuManager::State::Open) ? 405.0f : -10.0f; settings_menu_x != target_x) {

            if (const float delta = (target_x - settings_menu_x) * animation_speed * ImGui::GetIO().DeltaTime; std::abs(delta) < 0.1f) {
                settings_menu_x = target_x;
            } else {
                settings_menu_x += delta;
            }
        }
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(-10.0f, 40.0f), ImVec2(settings_menu_x, ImGui::GetWindowHeight()), ImColor(15.0f / 255.0f, 15.0f / 255.0f, 15.0f / 255.0f, 0.75f), 0.0f);

        const auto text_size = ImGui::CalcTextSize("Tools").x / 2;
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 202.5f - text_size, 60.0f));
        ImGui::Text("Tools");
        ImGui::GetWindowDrawList()->AddLine(ImVec2(settings_menu_x - 395.0f, 100.0f), ImVec2(settings_menu_x - 10.0f, 100.0f), ImColor(1.0f, 1.0f, 1.0f, 1.0f), 2.0f);

#ifdef WIN32
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 110.0f));
        if (ImGui::Button("Package Builder")) {
            if (auto result = static_cast<Router *>(*router)->setPage(0); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 160.0f));
        if (ImGui::Button("Package Differ")) {
            if (auto result = static_cast<Router *>(*router)->setPage(1); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 210.0f));
        if (ImGui::Button("Release Publisher")) {
            if (auto result = static_cast<Router *>(*router)->setPage(2); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 260.0f));
        if (ImGui::Button("Launcher JSON Manager")) {
            if (auto result = static_cast<Router *>(*router)->setPage(3); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }

        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 310.0f));
        if (ImGui::Button("Settings")) {
            if (auto result = static_cast<Router *>(*router)->setPage(4); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
#else
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 110.0f));
        if (ImGui::Button("Package Differ")) {
            if (auto result = static_cast<Router *>(*router)->setPage(0); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 160.0f));
        if (ImGui::Button("Release Publisher")) {
            if (auto result = static_cast<Router *>(*router)->setPage(1); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 210.0f));
        if (ImGui::Button("Launcher JSON Manager")) {
            if (auto result = static_cast<Router *>(*router)->setPage(2); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }
        ImGui::SetCursorPos(ImVec2(settings_menu_x - 230.0f, 260.0f));
        if (ImGui::Button("Settings")) {
            if (auto result = static_cast<Router *>(*router)->setPage(3); !result) {
                InfinityPackageBuilder::Errors::ShowErrorPopup(result.error());
            }
        }

#endif

        return settings_menu_x;
    }
    {
        std::cerr << "MenuUI::RenderSettingsMenu: Failed to obtain router instance" << std::endl;
        return 0.0f;
    }
}
