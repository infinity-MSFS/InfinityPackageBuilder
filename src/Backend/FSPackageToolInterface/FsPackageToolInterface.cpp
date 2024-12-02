
#include "FsPackageToolInterface.hpp"
#include "Util/Errors/Error.hpp"

#include <filesystem>
#include <future>

#include <Windows.h>

namespace Infinity {
    std::string FSPackageToolInterface::GetFSPackageToolPath() {

        std::filesystem::path path;

        char *fspath = nullptr;
        size_t length = 0;
        if (_dupenv_s(&fspath, &length, "MSFS_SDK") != 0 || fspath == nullptr) {
            Error(ErrorType::Warning, "Failed to get the path of the MSFS_SDK environment variable").Dispatch();
        } else {
            path = std::filesystem::path(fspath) / "Tools" / "bin" / "fspackagetool.exe";
            free(fspath);
        }
        return path.string();
    }

    void FSPackageToolInterface::StartCommand(std::string command_path, std::string project_xml_path, std::string output_dir) {
        if (a_is_running)
            return;


        a_is_running = true;
        std::thread worker([&, command_path, project_xml_path, output_dir]() {
            InvokeFSPackageTool(command_path, project_xml_path, output_dir);
            a_is_running = false;
        });
        worker.detach();
    }

    bool FSPackageToolInterface::GetIsLoading() { return a_is_running; }

    void FSPackageToolInterface::InvokeFSPackageTool(std::string command_path, std::string project_xml_path, std::string output_dir) {

        std::string command_string = command_path + " \"" + project_xml_path + "\" -outputdir \"" + output_dir + "\"" + " -nopause";
        std::cout << "Command: " << command_string << std::endl;
        FILE *pipe = _popen(command_string.c_str(), "r");
        if (!pipe)

            return;


        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            {
                std::cout << "Buffer: " << buffer << std::endl;
            }
            m_CV.notify_one();
        }
        _pclose(pipe);
    }


} // namespace Infinity
