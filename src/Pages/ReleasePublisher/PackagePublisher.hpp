
#pragma once
#include "Pages/Page.hpp"
#include "Util/GithubOAuth.hpp"
#include "Util/GithubRelease.hpp"
#include "Util/LuaInterpreter.hpp"

class PackagePublisher final : public Page {
public:
    PackagePublisher(float padding_x, float padding_y, const char *lua_file_path);

    void RenderPage() override;


private:
    InfinityPackageBuilder::Lua::LuaInterpreter m_LuaInterpreter;
    GithubOAuth m_Oauth;
    GithubRelease m_ReleaseManager;
};
