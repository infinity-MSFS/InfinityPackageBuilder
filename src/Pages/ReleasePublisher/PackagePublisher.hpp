
#pragma once
#include "Pages/Page.hpp"
#include "Util/LuaInterpreter.hpp"


class PackagePublisher final : public Page {
public:
    PackagePublisher(float padding_x, float padding_y);

    void RenderPage() override;

private:
    InfinityPackageBuilder::Lua::LuaInterpreter m_LuaInterpreter;
};
