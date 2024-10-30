#include <iostream>
#include "renderer/GUI/ApplicationGui.hpp"

bool g_ApplicationRunning = true;

static float s_elapsedTime = 0.0f;

static std::chrono::steady_clock::time_point s_lastTimestamp = std::chrono::steady_clock::now();

static float launchButtonOffset = 0.0f;
static const float animationSpeed = 5.0f;


class MainLayer : public InfinityRenderer::Layer {
public:
    virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        auto start = std::chrono::steady_clock::now();

        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(start - s_lastTimestamp).count() / 1000.0f;

        s_lastTimestamp = start;

        ImGui::Text("Marika");
    }
};

InfinityRenderer::Application *InfinityRenderer::CreateApplication(int argc, char **argv) {
    std::filesystem::path path = "Resources/Images/Logo.h";
    InfinityRenderer::ApplicationSpecifications spec = {
        "Infinity Package Manager",
        400,
        600,
        path,
        false,
        true,
        true
    };

    auto app = new InfinityRenderer::Application(spec);
    app->PushLayer<MainLayer>();

    return app;
}

namespace InfinityRenderer {
    int Main(int argc, char **argv) {
        while (g_ApplicationRunning) {
            auto app = InfinityRenderer::CreateApplication(argc, argv);
            app->Run();
            delete app;
        }
        return 0;
    }
}

#if defined(RELEASE_DIST)

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    return Marika::Main(__argc, __argv);
}

#else

int main(int argc, char **argv) {
    return InfinityRenderer::Main(argc, argv);
}

#endif
