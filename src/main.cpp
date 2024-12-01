#include <iostream>

#include "Backend/Application/Application.hpp"
#include "Backend/DifferPatcher/TestDiffPatch.hpp"
#include "Backend/PersistentState/PersistentState.hpp"
#include "Frontend/Components/Background/Background.hpp"
#include "Frontend/Components/Menu/Menu.hpp"
#include "Frontend/Pages/LauncherJsonManager/LauncherJsonManager.hpp"
#include "Frontend/Pages/PackageBuilder/PackageBuilder.hpp"
#include "Frontend/Pages/PackageDiffer/PackageDiffer.hpp"
#include "Frontend/Pages/ReleasePublisher/PackagePublisher.hpp"
#include "Frontend/Pages/Settings/Settings.hpp"
#include "Util/Router/Router.hpp"


bool g_ApplicationRunning = true;

namespace Infinity {

    class PageRenderLayer final : public Layer {
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

        void OnAttach() override {

            // test differ on load
           TestBSDiff();


            auto state = PersistentState::GetInstance();
            state->LoadState();
            Infinity::Application::SetWindowTitle(std::get<0>(buttons.front()));
        }
    };


    Application *CreateApplication(int argc, char **argv) {
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
} // namespace Infinity

#if defined(RELEASE_DIST) && WIN32

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd) { Infinity::Main(__argc, __argv); }

#else

int main(const int argc, char **argv) { Infinity::Main(argc, argv); }

#endif
