
#pragma once

#include "StreamReader.hpp"
#include "StreamWriter.hpp"

#include <fstream>
#include <filesystem>


namespace InfinityRenderer {
    class FileStreamWriter final : public StreamWriter {
    public:
        explicit FileStreamWriter(const std::filesystem::path &path);

        FileStreamWriter(const FileStreamWriter &) = delete;

        ~FileStreamWriter() override;

        [[nodiscard]] bool IsStreamGood() const final { return m_Stream.good(); }
        uint64_t GetStreamPosition() final { return m_Stream.tellp(); }
        void SetStreamPosition(const uint64_t position) final { m_Stream.seekp(position); }

        bool WriteData(const char *data, size_t size) final;

    private:
        std::filesystem::path m_Path;
        std::ofstream m_Stream;
    };

    class FileStreamReader final : public StreamReader {
    public:
        explicit FileStreamReader(const std::filesystem::path &path);

        FileStreamReader(const FileStreamReader &) = delete;

        ~FileStreamReader() override;

        [[nodiscard]] bool IsStreamGood() const final { return m_Stream.good(); }
        uint64_t GetStreamPosition() override { return m_Stream.tellg(); }
        void SetStreamPosition(const uint64_t position) override { m_Stream.seekg(position); }

        bool ReadData(char *destination, size_t size) override;

    private:
        std::filesystem::path m_Path;
        std::ifstream m_Stream;
    };
}

