
#pragma once

#include "Router/Router.hpp"
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


class Menu final : public InfinityRenderer::Layer {
public:
    void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        auto &router = InfinityPackageBuilder::Utils::Router::getInstance();
        // draw the icon thats clickable,
        if (ImGui::Button("Menu")) {
            MenuManager::toggleState();
        }

        if (MenuManager::getSate() == MenuManager::State::Open) {
            // draw the menu
            // rect

            if (ImGui::Button("Package Builder")) {
                router.setPage(0);
            }
            if (ImGui::Button("Package Differ")) {
                router.setPage(1);
            }
            if (ImGui::Button("Release Publisher")) {
                router.setPage(2);
            }
            if (ImGui::Button("Launcher JSON Manager")) {
                router.setPage(3);
            }
            if (ImGui::Button("Settings")) {
                router.setPage(4);
            }
        }
    }
};
