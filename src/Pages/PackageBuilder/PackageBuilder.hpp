
#pragma once
#include "Pages/Page.hpp"


class PackageBuilder final : public Page {
public:
    PackageBuilder(float padding_x, float padding_y);

    void RenderPage() override;
};
