#include <iostream>

#include "Components/Menu/Menu.hpp"
#include "renderer/GUI/ApplicationGui.hpp"

bool g_ApplicationRunning = true;

class PageRenderLayer final : public InfinityRenderer::Layer {
public:
    void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) override {
        auto &router = InfinityPackageBuilder::Utils::Router::getInstance();
        router.RenderCurrentPage();
    }
};


InfinityRenderer::Application *InfinityRenderer::CreateApplication(int argc, char **argv) {
    const std::filesystem::path path = "Resources/Images/Logo.h";
    const InfinityRenderer::ApplicationSpecifications spec = {
            "Infinity Package Manager", 1440, 1026, path, true, true, true};

    const auto app = new InfinityRenderer::Application(spec);
    app->PushLayer<PageRenderLayer>();
    app->PushLayer<Menu>();

    return app;
}
void PackageBuilder() { ImGui::Text("PackageBuilder"); }
void PackageDiffer() { ImGui::Text("PackageDiffer"); }
void ReleasePublisher() { ImGui::Text("ReleasePublisher"); }
void JsonManager() { ImGui::Text("JsonManager"); }
void Settings() { ImGui::Text("Settings"); }


namespace InfinityRenderer {
    int Main(const int argc, char **argv) {
        std::unordered_map<int, std::function<void()>> pages = {
                {0, PackageBuilder}, {1, PackageDiffer}, {2, ReleasePublisher}, {3, JsonManager}, {4, Settings}};
        InfinityPackageBuilder::Utils::Router::configure(pages);
        while (g_ApplicationRunning) {
            const auto app = InfinityRenderer::CreateApplication(argc, argv);
            app->Run();
            delete app;
        }
        return 0;
    }
} // namespace InfinityRenderer

#if defined(RELEASE_DIST)

#include <windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    return InfinityRenderer::Main(__argc, __argv);
}

#else

int main(int argc, char **argv) { return InfinityRenderer::Main(argc, argv); }

#endif
