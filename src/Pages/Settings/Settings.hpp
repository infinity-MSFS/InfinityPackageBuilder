
#pragma once

#include "Pages/Page.hpp"

#ifdef WIN32
#include "wintoastlib.h"
#endif

class Settings final : public Page {
public:
    Settings(float padding_x, float padding_y);

    void RenderPage() override;

private:
#ifdef WIN32
    WinToastLib::WinToastTemplate m_WinToast;
#endif
};
