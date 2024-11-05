
#pragma once


#include "imgui.h"
#include "vector"

static std::vector<ImVec2> m_circlePos;

class Background {
public:
    Background();

    void RenderBackground() const;

private:
    void RenderBackgroundDotsLayer() const;

    void RenderBackgroundGradientLayer() const;

    void RenderBackgroundBaseLayer() const;

    void RenderGradientCircle(ImVec2 center, float radius, float maxOpacity, ImU32 color) const;

private:
    ImDrawList *m_drawList;
    ImVec2 m_windowPos;
    ImVec2 m_windowSize;

    static void InitializeCirclePosition(const int index, const ImVec2 position) {
        if (index >= 0 && index < m_circlePos.size() && m_circlePos[index].x == 0 && m_circlePos[index].y == 0) {
            m_circlePos[index] = position;
        }
    }

    static void TrySetDefaultPositions();

    static ImVec2 GetCircleCoords(const float radius, const float theta, const ImVec2 center) {
        const float radians = (3.141592f / 180.0f) * theta;
        float x = center.x + radius * cosf(radians);
        float y = center.y + radius * sinf(radians);

        return {x, y};
    }
};
