#include <iostream>
#include "renderer/GUI/ApplicationGui.hpp"

bool g_ApplicationRunning = true;

class BackgroundLayer final : public InfinityRenderer::Layer {
public:
    void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        ImGui::Text("They are here");
    }
};


class ForegroundLayer final : public InfinityRenderer::Layer {
public:
    void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        ImGui::Text("The monkeys will rise");
    }
};

InfinityRenderer::Application *InfinityRenderer::CreateApplication(int argc, char **argv) {
    const std::filesystem::path path = "Resources/Images/Logo.h";
    const InfinityRenderer::ApplicationSpecifications spec = {
        "Infinity Package Manager",
        400,
        600,
        path,
        false,
        true,
        true
    };

    const auto app = new InfinityRenderer::Application(spec);
    app->PushLayer<BackgroundLayer>();
    app->PushLayer<ForegroundLayer>();

    return app;
}

namespace InfinityRenderer {
    int Main(const int argc, char **argv) {
        while (g_ApplicationRunning) {
            const auto app = InfinityRenderer::CreateApplication(argc, argv);
            app->Run();
            delete app;
        }
        return 0;
    }
}

#if defined(RELEASE_DIST)

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    return InfinityRenderer::Main(__argc, __argv);
}

#else

int main(int argc, char **argv) {
    return InfinityRenderer::Main(argc, argv);
}

#endif
