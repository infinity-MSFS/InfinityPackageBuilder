
#pragma once

#include "renderer/Layer.hpp"

class MenuManager {
public:
    enum class State { Open, Closed };

    static State getSate() { return m_CurrentState; }

    static void setState(const State state) { m_CurrentState = state; }

    static void toggleState() { m_CurrentState = (m_CurrentState == State::Open) ? State::Closed : State::Open; }


private:
    static State m_CurrentState;
};

struct LineParams {
    ImVec2 start;
    ImVec2 end;
};

struct SettingsIconAnimationParams {
    LineParams top;
    LineParams middle;
    LineParams bottom;
};


class MenuUI {
public:
    static bool RenderSettingsIcon(ImVec2 pos, MenuManager::State state);
    static float RenderSettingsMenu(MenuManager::State state);
};

class Menu final : public InfinityRenderer::Layer {
public:
    void OnUIRender() override {

        // we are extracting the setting window pos in the event that we need to move other elements to avoid
        // blocking / overlapping. If used we should move to a shared memory space / global state
        [[maybe_unused]] float settings_pos_x = MenuUI::RenderSettingsMenu(MenuManager::getSate());

#ifdef WIN32
        if (MenuUI::RenderSettingsIcon(ImVec2(10.0f, 50.0f), MenuManager::getSate())) {
            MenuManager::toggleState();
        }
#else
        if (MenuUI::RenderSettingsIcon(ImVec2(10.0f, 10.0f), MenuManager::getSate())) {
            MenuManager::toggleState();
        }
#endif
    }
};
