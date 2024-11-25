
#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "renderer/Layer.hpp"

#include "Router/Router.hpp"

using Infinity::Utils::Router;

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


#ifdef WIN32
static const std::vector<std::tuple<std::string, int, std::function<void()>, bool>> buttons = {{"Package Builder", 0,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(0); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Package Builder");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Package Differ", 1,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(1); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Package Differ");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Release Publisher", 2,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(2); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Release Publisher");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Launcher JSON Manager", 3,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(3); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Launcher JSON Manager");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Settings", 4,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(4); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Settings");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                true}};
#else
static const std::vector<std::tuple<std::string, int, std::function<void()>, bool>> buttons = {{"Package Differ", 0,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(0); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Package Differ");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Release Publisher", 1,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(1); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Release Publisher");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Launcher JSON Manager", 2,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(2); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Launcher JSON Manager");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                false},
                                                                                               {"Settings", 3,
                                                                                                [] {
                                                                                                    if (const auto router = Router::getInstance(); router.has_value()) {
                                                                                                        if (auto result = static_cast<Router *>(*router)->setPage(3); !result) {
                                                                                                            Infinity::Errors::ShowErrorPopup(result.error());
                                                                                                        } else {
                                                                                                            Infinity::Application::SetWindowTitle("Settings");
                                                                                                        }
                                                                                                    }
                                                                                                },
                                                                                                true}};

#endif

class Menu final : public Infinity::Layer {
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
