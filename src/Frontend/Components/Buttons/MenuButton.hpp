#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

namespace Infinity {
    class MenuButton {
    public:
        MenuButton(float x, float y, float width, float height, float spacing, const std::vector<std::tuple<std::string, int, std::function<void()>, bool>> &buttons);

        void UpdateLocation(float x, float y);

        void Render();

        void SetActive(const int active_id) { m_CurrentActiveID = active_id; };

    private:
        bool RenderButton(float x, float y, float width, float height, const char *label, int to, bool settings);

    private:
        float m_StartingX, m_StartingY, m_Width, m_Height, m_Spacing;

        std::vector<std::tuple<std::string, int, std::function<void()>, bool>> m_Buttons;

        int m_CurrentActiveID;
    };
} // namespace Infinity
