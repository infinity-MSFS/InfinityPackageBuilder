
#pragma once


#include <algorithm>
#include <cmath>
#include "imgui.h"

namespace Infinity {
    namespace Colors {
        static inline float Convert_sRGB_FromLinear(float theLinearValue);

        static inline float Convert_sRGB_ToLinear(float theRGBValue);

        ImVec4 ConvertFromSRGB(ImVec4 colour);

        ImVec4 ConvertToSRGB(ImVec4 colour);

        namespace Theme {
            constexpr auto accent = IM_COL32(236, 158, 36, 255);
            constexpr auto highlight = IM_COL32(39, 185, 242, 255);
            constexpr auto nice_blue = IM_COL32(83, 232, 254, 255);
            constexpr auto compliment = IM_COL32(78, 151, 166, 255);
            constexpr auto background = IM_COL32(36, 36, 36, 255);
            constexpr auto background_dark = IM_COL32(26, 26, 26, 255);
            constexpr auto title_bar = IM_COL32(21, 21, 21, 255);
            constexpr auto property_field = IM_COL32(15, 15, 15, 255);
            constexpr auto text = IM_COL32(192, 192, 192, 255);
            constexpr auto text_brighter = IM_COL32(210, 210, 210, 255);
            constexpr auto text_darker = IM_COL32(128, 128, 128, 255);
            constexpr auto text_error = IM_COL32(230, 51, 51, 255);
            constexpr auto muted = IM_COL32(77, 77, 77, 255);
            constexpr auto group_header = IM_COL32(47, 47, 47, 255);
            constexpr auto selection = IM_COL32(237, 192, 119, 255);
            constexpr auto selection_muted = IM_COL32(237, 201, 142, 23);
            constexpr auto background_popup = IM_COL32(50, 50, 50, 255);
            constexpr auto valid_prefab = IM_COL32(82, 179, 222, 255);
            constexpr auto invalid_prefab = IM_COL32(222, 43, 43, 255);
            constexpr auto missing_mesh = IM_COL32(230, 102, 76, 255);
            constexpr auto mesh_not_set = IM_COL32(250, 101, 23, 255);
        } // namespace Theme
    } // namespace Colors

    namespace Colors {
        inline float Convert_sRGB_FromLinear(const float theLinearValue) { return theLinearValue <= 0.0031308f ? theLinearValue * 12.92f : std::powf(theLinearValue, 1.0f / 2.2f) * 1.055f - 0.055f; }

        inline float Convert_sRGB_ToLinear(const float theRGBValue) { return theRGBValue <= 0.04045f ? theRGBValue / 12.92f : std::powf((theRGBValue + 0.055f) / 1.055f, 2.2f); }

        inline ImVec4 ConvertFromSRGB(ImVec4 colour) { return {Convert_sRGB_FromLinear(colour.x), Convert_sRGB_FromLinear(colour.y), Convert_sRGB_FromLinear(colour.z), colour.w}; }

        inline ImVec4 ConvertToSRGB(ImVec4 colour) { return {std::powf(colour.x, 2.2f), std::powf(colour.y, 2.2f), std::powf(colour.z, 2.2f), colour.w}; }

    } // namespace Colors

    void SetInfinityTheme();
} // namespace Infinity
