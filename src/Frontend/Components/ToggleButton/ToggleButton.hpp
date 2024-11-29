
#pragma once

#include "imgui_internal.h"

inline void ToggleButton(const char *str_id, bool *v) {
    const ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    const float height = ImGui::GetFrameHeight();
    const float width = height * 1.55f;
    const float radius = height * 0.50f;

    ImGui::InvisibleButton(str_id, ImVec2(width, height));
    if (ImGui::IsItemClicked())
        *v = !*v;

    float t = *v ? 1.0f : 0.0f;

    if (const ImGuiContext &g = *GImGui; g.LastActiveId == g.CurrentWindow->GetID(str_id)) {
        constexpr float animation_speed = 0.08f;
        const float t_anim = ImSaturate(g.LastActiveIdTimer / animation_speed);
        t = *v ? (t_anim) : (1.0f - t_anim);
    }

    ImU32 col_bg;
    if (ImGui::IsItemHovered())
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.78f, 0.78f, 0.78f, 1.0f), ImVec4(0.0f, 1.0f, 0.63f, 1.0f), t));
    else
        col_bg = ImGui::GetColorU32(ImLerp(ImVec4(0.85f, 0.85f, 0.85f, 1.0f), ImVec4(0.0f, 1.0f, 0.45f, 1.0f), t));

    draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
    draw_list->AddCircleFilled(ImVec2(p.x + radius + t * (width - radius * 2.0f), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}
