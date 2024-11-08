#pragma once

#include <chrono>
#include <functional>


class StateTimer {
public:
    StateTimer() : m_StartTime(), m_Duration(static_cast<long>(0)), m_Active(false) {}

    void Start(int durationInSeconds) {
        m_StartTime = std::chrono::steady_clock::now();
        m_Duration = std::chrono::seconds(static_cast<long>(durationInSeconds));
        m_Active = true;
    }

    void Update() {
        if (m_Active) {
            auto now = std::chrono::steady_clock::now();
            if (now - m_StartTime >= m_Duration) {
                m_Active = false;
            }
        }
    }

    [[nodiscard]] bool IsActive() const { return m_Active; }

private:
    std::chrono::steady_clock::time_point m_StartTime;
    std::chrono::seconds m_Duration;
    bool m_Active;
};
