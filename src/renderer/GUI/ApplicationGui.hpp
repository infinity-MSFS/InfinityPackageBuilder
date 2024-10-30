
#pragma once

#include "renderer/Layer.hpp"
#include "renderer/image/Image.hpp"
#include "renderer/GUI/UI/UI.hpp"
#include "ImGuiTheme/ImGuiTheme.hpp"

#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
#include <functional>
#include <filesystem>

#include "imgui.h"
#include "vulkan/vulkan.h"


void check_vk_result(VkResult err);

struct GLFWwindow;

namespace InfinityRenderer {
    struct ApplicationSpecifications {
        std::string Name;
        uint32_t Width;
        uint32_t Height;

        std::filesystem::path IconPath;

        bool WindowResizable;
        bool CustomTitlebar;
        bool CenterWindow;
    };

    class Application {
    public:
        explicit Application(const ApplicationSpecifications &applicationSpecification);

        ~Application();

        static Application &Get();

        void Run();

        void SetMenubarCallback(const std::function<void()> &menubarCallback) { m_MenubarCallback = menubarCallback; }

        template<typename T>
        void PushLayer() {
            static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
            m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
        }

        void PushLayer(const std::shared_ptr<Layer> &layer) {
            m_LayerStack.emplace_back(layer);
            layer->OnAttach();
        }

        void Close();

        [[nodiscard]] bool IsMaximized() const;

        [[nodiscard]] std::shared_ptr<Image> GetApplicationIcon() const { return m_AppHeaderIcon; }

        float GetTime();

        [[nodiscard]] GLFWwindow *GetWindowHandle() const { return m_WindowHandle; }
        [[nodiscard]] bool IsTitleBarHovered() const { return m_TitleBarHovered; }

        static VkInstance GetInstance();

        static VkPhysicalDevice GetPhysicalDevice();

        static VkDevice GetDevice();

        static VkCommandBuffer GetCommandBuffer(bool begin);

        static void FlushCommandBuffer(VkCommandBuffer commandBuffer);

        static void SubmitResourceFree(std::function<void()> &&func);

        static ImFont *GetFont(const std::string &name);

        template<typename Func>
        void QueueEvent(Func &&func) {
            m_EventQueue.push(func);
        }

    private:
        void Init();

        void Shutdown();

        void SetWindowIcon(GLFWwindow *window, const unsigned char *data, int size);

        void UI_DrawTitlebar(float &outTitlebarHeight);

        void UI_DrawMenubar();

    private:
        ApplicationSpecifications m_Specification;
        GLFWwindow *m_WindowHandle = nullptr;
        bool m_Running = false;

        float m_TimeStep = 0.0f;
        float m_FrameTime = 0.0f;
        float m_LastFrameTime = 0.0f;

        bool m_TitleBarHovered = false;

        std::vector<std::shared_ptr<Layer> > m_LayerStack;
        std::function<void()> m_MenubarCallback;

        std::mutex m_EventQueueMutex;
        std::queue<std::function<void()> > m_EventQueue;


        std::shared_ptr<Image> m_AppHeaderIcon;
        std::shared_ptr<Image> m_IconClose;
        std::shared_ptr<Image> m_IconMinimize;
        std::shared_ptr<Image> m_IconMaximize;
        std::shared_ptr<Image> m_IconRestore;
    };

    Application *CreateApplication(int argc, char **argv);
}
