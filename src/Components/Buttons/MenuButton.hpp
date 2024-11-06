#pragma once


class MenuButton {
public:
    MenuButton(float x, float y, float width, float height, int active_id);

    bool Render();

    void SetActive(int active_id) { m_CurrentActiveID = active_id; };

private:
    [[nodiscard]] bool IsAactive() const {
        if (m_CurrentActiveID == m_ActiveId) {
            return true;
        } else {
            return false;
        }
    }

private:
    float m_X, m_Y, m_Width, m_Height;

    bool m_Clicked, m_Hovered;

    int m_ActiveId, m_CurrentActiveID;
};
