
#pragma once
#include "Pages/Page.hpp"


class LauncherJsonManager final : public Page {
public:
    LauncherJsonManager(float padding_x, float padding_y);

    void RenderPage() override;
};
