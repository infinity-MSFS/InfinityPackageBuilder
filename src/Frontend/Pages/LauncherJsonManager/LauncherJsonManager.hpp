
#pragma once
#include "Frontend/Components/SelectKeyFromFile/SelectKeyFromFile.hpp"
#include "Frontend/Pages/Page.hpp"

namespace Infinity {
    class LauncherJsonManager final : public Page {
    public:
        LauncherJsonManager(float padding_x, float padding_y);

        void RenderPage() override;

    private:
        Infinity::SelectKeyFromFile m_FileSelector;
    };
} // namespace Infinity
