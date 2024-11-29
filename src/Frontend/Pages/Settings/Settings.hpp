
#pragma once

#include "Frontend/Pages/Page.hpp"


namespace Infinity {
    class Settings final : public Page {
    public:
        Settings(float padding_x, float padding_y);

        void RenderPage() override;
    };
} // namespace Infinity
