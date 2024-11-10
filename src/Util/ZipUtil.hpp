
#pragma once

#include <filesystem>
#include <iostream>
#include <minizip/zip.h>
#include <string>


class ZipUtil {
public:
    ZipUtil() = default;

    static bool ZipDirectory(const std::string &dir_path, const std::string &zip_path);

    static bool UnzipToDirectory(const std::string &zip_path, const std::string &dir_path);

private:
    static void AddFileToZip(zipFile zf, const std::filesystem::path &file_path, const std::filesystem::path &base_path);
};
