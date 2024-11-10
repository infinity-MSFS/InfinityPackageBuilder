
#pragma once

#ifdef WIN32
#include <Windows.h>
#endif
#include <cstdlib>
#include <string>

inline void OpenUrlInBrowser(std::string &url) {
#ifdef WIN32
    ShellExecute(nullptr, "open", url.c_str(), nullptr, nullptr, SW_HIDE);

#elif __APPLE__
    std::string command = "open " + url;
    std::system(command.cstr());
#elif __linux__
    std::string command = "open " + url;
    std::system(command.cstr());
#endif
}
