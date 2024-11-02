
#pragma once
#include "Pages/Page.hpp"


class Settings final : public Page {
public:
    Settings(float padding_x, float padding_y);

    void RenderPage() override;
};
