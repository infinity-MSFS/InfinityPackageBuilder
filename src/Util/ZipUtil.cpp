
#include "ZipUtil.hpp"
#include <fstream>
#include <minizip/unzip.h>
#include "zoe/zoe.h"

bool ZipUtil::ZipDirectory(const std::string &dir_path, const std::string &zip_path) {
    const zipFile zf = zipOpen(zip_path.c_str(), APPEND_STATUS_CREATE);
    if (!zf) {
        std::cerr << "Failed to create Zip file:" << zip_path << std::endl;
        return false;
    }
    try {
        for (const std::filesystem::path base_path(dir_path); const auto &entry: std::filesystem::recursive_directory_iterator(base_path)) {
            if (entry.is_regular_file()) {
                AddFileToZip(zf, entry.path(), base_path);
            }
        }
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << "Fs error: " << e.what() << std::endl;
        zipClose(zf, nullptr);
        return false;
    }
    zipClose(zf, nullptr);
    return true;
}


void ZipUtil::AddFileToZip(const zipFile zf, const std::filesystem::path &file_path, const std::filesystem::path &base_path) {
    const std::string relative_path = std::filesystem::relative(file_path, base_path).string();

    constexpr zip_fileinfo info = {};
    zipOpenNewFileInZip(zf, relative_path.c_str(), &info, nullptr, 0, nullptr, 0, nullptr, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

    std::ifstream input_file(relative_path, std::ios::binary);
    std::vector<char> buffer(1024);
    while (input_file.read(buffer.data(), static_cast<long long>(buffer.size())) || input_file.gcount() > 0) {
        zipWriteInFileInZip(zf, buffer.data(), static_cast<unsigned int>(input_file.gcount()));
    }
    zipCloseFileInZip(zf);
}

bool ZipUtil::UnzipToDirectory(const std::string &zip_path, const std::string &dir_path) {
    const unzFile unz_file = unzOpen(zip_path.c_str());
    if (!unz_file) {
        std::cerr << "Failed to open zip file" << std::endl;
        return false;
    }

    if (unzGoToFirstFile(unz_file) != UNZ_OK) {
        std::cerr << "Could not read first file" << std::endl;
        unzClose(unz_file);
        return false;
    }

    do {
        unz_file_info file_info;
        char file_name[256];
        unzGetCurrentFileInfo(unz_file, &file_info, file_name, sizeof(file_name), nullptr, 0, nullptr, 0);

        std::string full_path = dir_path + "/" + file_name;
        if (file_name[file_info.size_filename - 1] == '/') {
            std::filesystem::create_directories(full_path);
        } else {
            std::filesystem::create_directories(std::filesystem::path(full_path).parent_path());

            unzOpenCurrentFile(unz_file);
            std::ofstream outputFile(full_path, std::ios::binary);
            std::vector<char> buffer(1024);
            int bytesRead;
            while ((bytesRead = unzReadCurrentFile(unz_file, buffer.data(), buffer.size())) > 0) {
                outputFile.write(buffer.data(), bytesRead);
            }
            unzCloseCurrentFile(unz_file);
        }
    } while (unzGoToNextFile(unz_file) != UNZ_END_OF_LIST_OF_FILE);

    unzClose(unz_file);
    return true;
}
