
#pragma once

#include "imgui.h"


#include "Util/Encryption/Encryption.hpp"

#include <vector>
#include <string>

#ifdef WIN32
#include <mutex>
#include <shobjidl.h>
#include <thread>
#include <windows.h>

inline std::mutex filePathMutex;

inline void OpenFileDialogThread(std::string& outputFilePath) {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        return;
    }

    IFileOpenDialog* pFileOpen = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL,
                          IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        COMDLG_FILTERSPEC rgSpec[] =
        {
            { L"Infinity Key Files", L"*.infinitykey" },
            { L"All Files", L"*.*" }
        };

        hr = pFileOpen->SetFileTypes(
            _countof(rgSpec),
            rgSpec
        );

        if (SUCCEEDED(hr)) {
            hr = pFileOpen->SetDefaultExtension(L"infinitykey");
            if (!SUCCEEDED(hr))
                std::cerr << "Failed to set default extension." << std::endl;
        }

        hr = pFileOpen->Show(nullptr);

        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if (SUCCEEDED(hr)) {
                    const int wideLength = wcslen(pszFilePath);
                    const int utf8Length = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, wideLength, nullptr, 0, nullptr, nullptr);

                    std::string filePath(utf8Length, 0);
                    WideCharToMultiByte(CP_UTF8, 0, pszFilePath, wideLength,
                                        &filePath[0], utf8Length, nullptr, nullptr);
                    {
                        std::lock_guard<std::mutex> lock(filePathMutex);
                        outputFilePath = filePath;
                    }
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }
    CoUninitialize();
}


inline void ShowFileDialog(std::string& filePath) {
    std::thread dialogThread(OpenFileDialogThread, std::ref(filePath));
    dialogThread.detach();
}
#elif __linux__
#include <cstdio>
#include <cstdlib>
#include <array>
#include <memory>
#include <thread>
#include <mutex>
#include <stdio.h>

inline std::mutex file_path_mutex;

inline std::string ExecCommand(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"),popen);

    if (!pipe) {
        Infinity::Errors::Error(Infinity::Errors::ErrorType::NonFatal, "popen() failed").Dispatch();
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }

    return result;
}


inline std::string OpenFileDialog() {
    try {
        const std::string cmd = R"(kdialog --getopenfilename . "*.infinitykey|Infinity Key Files" "*.infinitykey")";
        return ExecCommand(cmd.c_str());
    } catch (const std::exception& e) {
        Infinity::Errors::Error(Infinity::Errors::ErrorType::NonFatal, e.what());
        return "";
    }
}

inline void ShowFileDialog(std::string& filePath) {
    std::thread dialogThread([&filePath]() {
        std::lock_guard<std::mutex> lock(file_path_mutex);
        filePath = OpenFileDialog();
    });
    dialogThread.detach();
}

#else
inline void ShowFileDialog(std::string& filePath) {
    Infinity::Errors::Error(Infinity::Errors::ErrorType::NonFatal, "Unsupported Platform, please use clipboard");
}


#endif

namespace Infinity {
    class SelectKeyFromFile {
public:
        SelectKeyFromFile();

        void Render();

        private:
        void LoadKeyFromFile();

        private:
        bool m_RememberKeyFromFile;
        std::vector<uint8_t> m_KeyData;
        std::string m_FilePath;
    };

}
