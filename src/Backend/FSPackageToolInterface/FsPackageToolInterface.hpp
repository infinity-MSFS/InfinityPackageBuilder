
#pragma once

#include <condition_variable>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace Infinity {

    class FSPackageToolInterface {
    public:
        FSPackageToolInterface() : a_is_running(false) {}

        static std::string GetFSPackageToolPath();
        void StartCommand(std::string command_path, std::string project_xml_path, std::string output_dir);
        bool GetIsLoading();

    private:
        void InvokeFSPackageTool(std::string command_path, std::string project_xml_path, std::string output_dir);

    private:
        std::atomic<bool> a_is_running;
        std::condition_variable m_CV;
    };
} // namespace Infinity
