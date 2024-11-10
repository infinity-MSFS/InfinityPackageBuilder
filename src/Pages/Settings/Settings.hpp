
#pragma once
#include "Pages/Page.hpp"
#include "wintoastlib.h"


class Settings final : public Page {
public:
    Settings(float padding_x, float padding_y);

    void RenderPage() override;

private:
    WinToastLib::WinToastTemplate m_WinToast;
};
