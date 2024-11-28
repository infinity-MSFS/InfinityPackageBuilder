
#pragma once
#include "Pages/Page.hpp"
#include "Components/SelectKeyFromFile/SelectKeyFromFile.hpp"


class LauncherJsonManager final : public Page {
public:
    LauncherJsonManager(float padding_x, float padding_y);

    void RenderPage() override;

    private:
    Infinity::SelectKeyFromFile m_FileSelector;
};
