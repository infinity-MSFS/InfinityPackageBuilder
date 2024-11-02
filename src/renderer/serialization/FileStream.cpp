
#include "FileStream.hpp"

namespace InfinityRenderer {
    FileStreamWriter::FileStreamWriter(const std::filesystem::path &path) : m_Path(path) {
        m_Stream = std::ofstream(path, std::ifstream::out | std::ifstream::binary);
    }

    FileStreamWriter::~FileStreamWriter() { m_Stream.close(); }

    bool FileStreamWriter::WriteData(const char *data, const size_t size) {
        m_Stream.write(data, size);
        return true;
    }


    FileStreamReader::FileStreamReader(const std::filesystem::path &path) : m_Path(path) {
        m_Stream = std::ifstream(path, std::ifstream::in | std::ifstream::binary);
    }

    FileStreamReader::~FileStreamReader() { m_Stream.close(); }

    bool FileStreamReader::ReadData(char *destination, const size_t size) {
        m_Stream.read(destination, size);
        return true;
    }
} // namespace InfinityRenderer
