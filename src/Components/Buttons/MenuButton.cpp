#include "MenuButton.hpp"

#include <imgui.h>
#include "Router/Router.hpp"
#include "imgui.h"
#include "imgui_internal.h"


MenuButton::MenuButton(const float x, const float y, const float width, const float height, const float spacing,
                       const std::vector<std::tuple<std::string, int, std::function<void()>, bool>> &buttons) :
    m_StartingX(x), m_StartingY(y), m_Width(width), m_Height(height), m_Spacing(spacing), m_Buttons(buttons), m_CurrentActiveID(0) {}


void MenuButton::UpdateLocation(float x, float y) {
    m_StartingX = x;
    m_StartingY = y;
}

bool MenuButton::RenderButton(float x, float y, float width, float height, const char *label, int to, bool settings) {
    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems) {
        return false;
    }
    ImGuiContext &ctx = *GImGui;
    const ImGuiStyle &style = ctx.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 labelSize = ImGui::CalcTextSize(label, nullptr, NULL);

    ImVec2 innerSize = {width, height};
    if (innerSize.x <= 0.0f) {
        innerSize.x = labelSize.x + style.FramePadding.x * 2.0f;
    }
    if (innerSize.y <= 0.0f) {
        innerSize.y = labelSize.y + style.FramePadding.y * 2.0f;
    }

    const ImRect rct({x, y}, ImVec2(x + innerSize.x, y + innerSize.y));
    ImGui::ItemSize(rct, style.FramePadding.y);
    if (!ImGui::ItemAdd(rct, id)) {
        return false;
    }
    bool hovered, held;
    const bool pressed = ImGui::ButtonBehavior(rct, id, &hovered, &held);

    if (m_CurrentActiveID == to) {
        if (settings) {
            window->DrawList->AddRectFilled(rct.Min, rct.Max, ImColor(0.5f, 0.5f, 1.0f, 0.5f), 8, NULL);
            ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 1.0f, 1.0f, 1.0f});
        } else {
            window->DrawList->AddRectFilled(rct.Min, rct.Max, ImColor(1.0f, 1.0f, 1.0f, 0.7f), 8, NULL);
            ImGui::PushStyleColor(ImGuiCol_Text, {0.0f, 0.0f, 0.0f, 1.0f});
        }

    } else {
        ImGui::PushStyleColor(ImGuiCol_Text, {1.0f, 1.0f, 1.0f, 1.0f});
    }

    if (held || hovered) {
        if (settings) {
            window->DrawList->AddRect(rct.Min, rct.Max, ImColor(0.5f, 0.5f, 1.0f, 0.5f), 8, NULL, 2.0f);
        } else {
            window->DrawList->AddRect(rct.Min, rct.Max, ImColor(1.0f, 1.0f, 1.0f, 0.7f), 8, NULL, 2.0f);
        }

    } else {
        if (settings) {
            window->DrawList->AddRect(rct.Min, rct.Max, ImColor(0.5f, 0.5f, 1.0f, 0.7f), 8, NULL, 2.0f);
        } else {
            window->DrawList->AddRect(rct.Min, rct.Max, ImColor(1.0f, 1.0f, 1.0f, 1.0f), 8, NULL, 2.0f);
        }
    }


    ImGui::RenderTextClipped(ImVec2(rct.Min.x + style.FramePadding.x, rct.Min.y + style.FramePadding.y), ImVec2(rct.Max.x - style.FramePadding.x, rct.Max.y - style.FramePadding.y), label, NULL,
                             &labelSize, style.ButtonTextAlign, &rct);

    ImGui::PopStyleColor();
    if (pressed) {
        m_CurrentActiveID = to;
    }


    return pressed;
}


void MenuButton::Render() {
    if (const auto router = Infinity::Utils::Router::getInstance(); router.has_value()) {
        m_CurrentActiveID = (*router)->getPage().value_or(0);
    }
    int i = 0;
    for (const auto &button: m_Buttons) {
        ImVec2 pos;
        if (std::get<3>(button)) { // handle settings button being at bottom always
            pos = {m_StartingX, ImGui::GetWindowHeight() - m_Height - m_Spacing};
        } else {
            pos = {m_StartingX, m_StartingY + i * (m_Height + m_Spacing)};
        }

        if (RenderButton(pos.x, pos.y, m_Width, m_Height, std::get<0>(button).c_str(), std::get<1>(button), std::get<3>(button))) {
            std::get<2>(button)();
        };
        i++;
    }
}
