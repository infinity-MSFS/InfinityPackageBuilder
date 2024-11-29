
#include "Background.hpp"
#include <ranges>
#include <unordered_map>

namespace Infinity {
    const std::unordered_map<std::string, ImVec2> defaultCirclePositions = {
            {"Circle1", ImVec2(100, 100)}, {"Circle2", ImVec2(200, 200)}, {"Circle3", ImVec2(300, 300)}, {"Circle4", ImVec2(400, 400)}, {"Circle5", ImVec2(200, 50)}};


    Background::Background() : m_drawList(ImGui::GetWindowDrawList()), m_windowPos(ImGui::GetWindowPos()), m_windowSize(ImGui::GetWindowSize()) {
        if (m_circlePos.empty()) {
            m_circlePos.resize(5);
        }
    }

    void Background::RenderBackground() const {
        RenderBackgroundBaseLayer();
        RenderBackgroundGradientLayer();
        RenderBackgroundDotsLayer();
    }


    void Background::RenderBackgroundBaseLayer() const {
        m_drawList->AddRectFilled(m_windowPos, ImVec2(m_windowPos.x + m_windowSize.x, m_windowPos.y + m_windowSize.y), ImColor(0.0f, 0.0f, 0.0f, 1.0f));

        m_drawList->AddRectFilledMultiColor(m_windowPos, ImVec2(m_windowPos.x + m_windowSize.x, m_windowPos.y + m_windowSize.y), ImColor(108.0f / 255.0f, 0.0f, 162.0f / 255.0f, 0.2f),
                                            ImColor(108.0f / 255.0f, 0.0f, 162.0f / 255.0f, 0.2f), ImColor(0.0f, 17.0f / 255.0f, 82.0f / 255.0f, 0.2f),
                                            ImColor(0.0f, 17.0f / 255.0f, 82.0f / 255.0f, 0.2f));
    }

    void Background::RenderGradientCircle(const ImVec2 center, const float radius, const float maxOpacity, const ImU32 color) const {
        constexpr int layers = 60;

        const ImVec4 colorVec = ImGui::ColorConvertU32ToFloat4(color);

        for (int i = 0; i < layers; i++) {
            constexpr int segments = 100;
            const float layerRadius = radius * (layers - static_cast<float>(i)) / layers;
            float alpha = colorVec.w * (static_cast<float>(i) + 1.0f) / layers;

            if (alpha > maxOpacity) {
                alpha = maxOpacity;
            }

            const ImU32 layerColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colorVec.x, colorVec.y, colorVec.z, alpha));

            m_drawList->AddCircleFilled(center, layerRadius, layerColor, segments);
        }
    }

    void Background::TrySetDefaultPositions() {
        if (m_circlePos.size() == 5) {
            int i = 0;
            for (const auto &val: defaultCirclePositions | std::views::values) {
                InitializeCirclePosition(i++, val);
                if (i >= 5)
                    break;
            }
        }
    }

    void Background::RenderBackgroundDotsLayer() const {
        const auto offsetPosition = ImVec2(m_windowPos.x + 10, m_windowPos.y + 10);
        const auto dotCount2D = ImVec2(150, 150);
        const auto color = ImColor(1.0f, 1.0f, 1.0f, 0.3f);

        for (int y = 0; static_cast<float>(y) < dotCount2D.y; y++) {
            for (int x = 0; static_cast<float>(x) < dotCount2D.x; x++) {
                constexpr int radius = 1;
                constexpr float spacing = 20.0f;
                auto pos = ImVec2(offsetPosition.x + static_cast<float>(x) * spacing, offsetPosition.y + static_cast<float>(y) * spacing);
                m_drawList->AddCircleFilled(pos, radius, color);
            }
        }
    }

    void Background::RenderBackgroundGradientLayer() const {
        static float circle1angle = 0.0f;
        static float circle2angle = 0.0f;
        static float circle3angle = 0.0f;
        static float circle4angle = 0.0f;
        static float circle5angle = 0.0f;


        TrySetDefaultPositions();

        RenderGradientCircle(ImVec2(m_windowPos.x + m_circlePos[0].x, m_windowPos.y + m_circlePos[0].y), 600.0f, 0.01f, ImColor(18.0f / 255.0f, 113.0f / 255.f, 1.0f, 0.01f));
        RenderGradientCircle(ImVec2(m_windowPos.x + m_circlePos[1].x, m_windowPos.y + m_circlePos[1].y), 400.0f, 0.01f, ImColor(221.0f / 255.f, 74.0f / 255.f, 1.0f, 0.01f));
        RenderGradientCircle(ImVec2(m_windowPos.x + m_circlePos[2].x, m_windowPos.y + m_circlePos[2].y), 600.0f, 0.01f, ImColor(100.0f / 255.f, 220.0f / 255.f, 1.0f, 0.01f));
        RenderGradientCircle(ImVec2(m_windowPos.x + m_circlePos[3].x, m_windowPos.y + m_circlePos[3].y), 400.0f, 0.01f, ImColor(200.0f / 255.f, 50.0f / 255.f, 50.0f / 255.f, 0.01f));
        RenderGradientCircle(ImVec2(m_windowPos.x + m_circlePos[4].x, m_windowPos.y + m_circlePos[4].y), 600.0f, 0.01f, ImColor(180.0f / 255.f, 180.0f / 255.f, 50.0f / 255.f, 0.01f));

        if (circle1angle - 0.05f < 0.0f) {
            circle1angle = 360.0f;
        } else {
            circle1angle -= 0.05f;
        }
        if (circle2angle + 0.05f > 360) {
            circle2angle = 0;
        } else {
            circle2angle += 0.05f;
        }
        if (circle3angle - 0.05f < 0.0f) {
            circle3angle = 360.0f;
        } else {
            circle3angle -= 0.05f;
        }
        if (circle4angle + 0.05f > 360) {
            circle4angle = 0;
        } else {
            circle4angle += 0.05f;
        }
        if (circle5angle - 0.05f < 360) {
            circle5angle = 0;
        } else {
            circle5angle -= 0.05f;
        }

        const auto screen_center = ImVec2(ImGui::GetWindowSize().x / 2.0f, ImGui::GetWindowSize().y / 2.0f);

        m_circlePos[0] = GetCircleCoords(600.0f, circle1angle, ImVec2(screen_center.x - 100.0f, screen_center.y + 200.0f));
        m_circlePos[1] = GetCircleCoords(500.0f, circle2angle, ImVec2(screen_center.x + 100.0f, screen_center.y + 20.0f));
        m_circlePos[2] = GetCircleCoords(155.0f, circle3angle, ImVec2(screen_center.x - 200.0f, screen_center.y - 100.0f));
        m_circlePos[3] = GetCircleCoords(409.0f, circle4angle, ImVec2(screen_center.x + 0.0f, screen_center.y + 200.0f));
        m_circlePos[4] = GetCircleCoords(781.0f, circle5angle, ImVec2(screen_center.x - 100.0f, screen_center.y + 0.0f));
    }
} // namespace Infinity
