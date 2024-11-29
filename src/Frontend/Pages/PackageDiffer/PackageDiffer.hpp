
#pragma once
#include "Frontend/Pages/Page.hpp"


namespace Infinity {
    class PackageDiffer final : public Page {
    public:
        PackageDiffer(float padding_x, float padding_y);

        void RenderPage() override;
    };
} // namespace Infinity
