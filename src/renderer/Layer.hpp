
#pragma once

#include <imgui.h>

namespace InfinityRenderer {
    class Layer {
    public:
        virtual ~Layer() = default;

        virtual void OnAttach() {
        }

        virtual void OnDetach() {
        }

        virtual void OnUpdate(float ts) {
        }

        virtual void OnUIRender(ImVec2 windowPos, ImVec2 windowSize) {
        }
    };
}
