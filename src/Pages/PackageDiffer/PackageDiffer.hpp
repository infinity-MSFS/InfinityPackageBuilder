
#pragma once
#include "Pages/Page.hpp"


class PackageDiffer final : public Page {
public:
    PackageDiffer(float padding_x, float padding_y);

    void RenderPage() override;
};
