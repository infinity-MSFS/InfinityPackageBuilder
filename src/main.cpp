#include <iostream>

#include "Components/Background/Background.hpp"
#include "Components/Menu/Menu.hpp"
#include "Pages/LauncherJsonManager/LauncherJsonManager.hpp"
#include "Pages/PackageBuilder/PackageBuilder.hpp"
#include "Pages/PackageDiffer/PackageDiffer.hpp"
#include "Pages/ReleasePublisher/PackagePublisher.hpp"
#include "Pages/Settings/Settings.hpp"
#include "Router/Router.hpp"
#include "renderer/GUI/ApplicationGui.hpp"

bool g_ApplicationRunning = true;
using InfinityPackageBuilder::Utils::Router;

class PageRenderLayer final : public InfinityRenderer::Layer {
public:
    void OnUIRender() override {
        const Background background;

        background.RenderBackground();

        if (const auto router = Router::getInstance(); router.has_value()) {
            (*router)->RenderCurrentPage();
        } else {
            std::cerr << "PageRenderLayer: Cannot obtain router instance" << std::endl;
        }
    }

    void OnAttach() override { InfinityRenderer::Application::SetWindowTitle(std::get<0>(buttons.front())); }
};


InfinityRenderer::Application *InfinityRenderer::CreateApplication(int argc, char **argv) {
    const std::filesystem::path path = "Resources/Images/Logo.h";
    const ApplicationSpecifications spec = {"Infinity Package Manager",
                                            1440,
                                            1026,
                                            2560,
                                            1440,
                                            1240,
                                            680,
                                            path,
                                            true,
#ifdef WIN32
                                            true,
#else
                                            false,
#endif
                                            true};

    const auto app = new Application(spec);
    app->PushLayer<PageRenderLayer>();
    app->PushLayer<Menu>();

    return app;
}


namespace InfinityRenderer {

    int Main(const int argc, char **argv) {
#ifdef WIN32
        const auto package_builder = new PackageBuilder(30.0f, 30.0f);
#endif
        const auto package_differ = new PackageDiffer(30.0f, 30.0f);

#ifdef WIN32
        auto test_path = "C:\\test\\script.lua";
#else
        auto test_path = "~/.config/test/script.lua";
#endif


        const auto release_publisher = new PackagePublisher(30.0f, 30.0f, test_path);
        const auto launcher_json_manager = new LauncherJsonManager(30.0f, 30.0f);
        const auto settings = new Settings(30.0f, 30.0f);

#ifdef WIN32
        const std::unordered_map<int, std::function<void()>> pages = {{0, [package_builder]() { package_builder->RenderPage(); }},
                                                                      {1, [package_differ]() { package_differ->RenderPage(); }},
                                                                      {2, [release_publisher]() { release_publisher->RenderPage(); }},
                                                                      {3, [launcher_json_manager]() { launcher_json_manager->RenderPage(); }},
                                                                      {4, [settings]() { settings->RenderPage(); }}};

#else
        const std::unordered_map<int, std::function<void()>> pages = {{0, [package_differ]() { package_differ->RenderPage(); }},
                                                                      {1, [release_publisher]() { release_publisher->RenderPage(); }},
                                                                      {2, [launcher_json_manager]() { launcher_json_manager->RenderPage(); }},
                                                                      {3, [settings]() { settings->RenderPage(); }}};
#endif
        Router::configure(pages);
        while (g_ApplicationRunning) {
            const auto app = CreateApplication(argc, argv);
            app->Run();
            delete app;
            g_ApplicationRunning = false;
        }
#ifdef WIN32
        delete package_builder;
#endif
        delete package_differ;
        delete release_publisher;
        delete launcher_json_manager;
        delete settings;
        return 0;
    }
} // namespace InfinityRenderer

#if defined(RELEASE_DIST) && WIN32

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd) { InfinityRenderer::Main(__argc, __argv); }

#else

int main(const int argc, char **argv) { InfinityRenderer::Main(argc, argv); }

#endif
