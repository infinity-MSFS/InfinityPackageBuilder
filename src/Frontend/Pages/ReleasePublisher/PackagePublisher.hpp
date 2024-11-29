
#pragma once
#include "Backend/Lua/LuaInterpreter.hpp"
#include "Frontend/Pages/Page.hpp"
#include "Util/Github/GithubOAuth.hpp"
#include "Util/Github/GithubRelease.hpp"

namespace Infinity {
    class PackagePublisher final : public Page {
    public:
        PackagePublisher(float padding_x, float padding_y, const char *lua_file_path);

        void RenderPage() override;


    private:
        Infinity::LuaInterpreter m_LuaInterpreter;
        GithubOAuth m_Oauth;
        GithubRelease m_ReleaseManager;
    };
} // namespace Infinity
