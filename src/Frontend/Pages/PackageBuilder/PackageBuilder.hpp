
#pragma once
#include "Frontend/Pages/Page.hpp"

namespace Infinity {
    class PackageBuilder final : public Page {
    public:
        PackageBuilder(float padding_x, float padding_y);

        void RenderPage() override;
    };
} // namespace Infinity
