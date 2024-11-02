
#pragma once
#include "Pages/Page.hpp"


class PackagePublisher final : public Page {
public:
    PackagePublisher(float padding_x, float padding_y);

    void RenderPage() override;
};
