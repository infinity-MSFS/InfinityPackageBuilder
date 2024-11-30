
#pragma once

#include "imgui.h"


#include "Util/Encryption/Encryption.hpp"

#include <string>
#include <string_view>
#include <vector>

#ifdef WIN32
#include <mutex>
#include <shobjidl.h>
#include <thread>
#include <windows.h>

inline std::mutex filePathMutex;

inline void OpenFileDialogThread(std::string &outputFilePath) {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        return;
    }

    IFileOpenDialog *pFileOpen = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void **>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        COMDLG_FILTERSPEC rgSpec[] = {{L"Infinity Key Files", L"*.infinitykey"}, {L"All Files", L"*.*"}};

        hr = pFileOpen->SetFileTypes(_countof(rgSpec), rgSpec);

        if (SUCCEEDED(hr)) {
            hr = pFileOpen->SetDefaultExtension(L"infinitykey");
            if (!SUCCEEDED(hr))
                std::cerr << "Failed to set default extension." << std::endl;
        }

        hr = pFileOpen->Show(nullptr);

        if (SUCCEEDED(hr)) {
            IShellItem *pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if (SUCCEEDED(hr)) {
                    const int wideLength = wcslen(pszFilePath);
                    const int utf8Length = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, wideLength, nullptr, 0, nullptr, nullptr);

                    std::string filePath(utf8Length, 0);
                    WideCharToMultiByte(CP_UTF8, 0, pszFilePath, wideLength, &filePath[0], utf8Length, nullptr, nullptr);
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


inline void ShowFileDialog(std::string &filePath) {
    std::thread dialogThread(OpenFileDialogThread, std::ref(filePath));
    dialogThread.detach();
}
#elif __linux__
#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <mutex>
#include <thread>

inline std::mutex filePathMutex;

inline void OpenFileDialogThread(std::string &outputFilePath) {
    if (!QCoreApplication::instance()) {
        static int argc = 0;
        static char *argv[] = {nullptr};
        static QApplication tempApp(argc, argv);
    }

    QString selectedFile = QFileDialog::getOpenFileName(nullptr, "Select Infinity Key File", QString(), "Infinity Key Files (*.infinitykey);; All Files (*.*)");

    if (!selectedFile.isEmpty()) {
        std::lock_guard<std::mutex> lock(filePathMutex);
        outputFilePath = selectedFile.toStdString();
    }
}

inline void ShowFileDialog(std::string &filePath) {
    std::thread dialogThread(OpenFileDialogThread, std::ref(filePath));
    dialogThread.detach();
}

#else
inline void ShowFileDialog(std::string &filePath) { Infinity::Errors::Error(Infinity::Errors::ErrorType::NonFatal, "Unsupported Platform, please use clipboard"); }

#endif

namespace Infinity {

    inline bool EndsWith(std::string_view str, std::string_view suffix) { return str.size() >= suffix.size() && str.substr(str.size() - suffix.size()) == suffix; }

    class SelectKeyFromFile {
    public:
        SelectKeyFromFile();

        void Render();

    private:
        void LoadKeyFromFile();

        void SaveKey();

        static std::string LoadKey();

    private:
        bool m_RememberKeyFromFile;
        std::vector<uint8_t> m_KeyData;
        std::string m_FilePath;
        std::string m_PreviousTickFilePath;
    };

} // namespace Infinity
