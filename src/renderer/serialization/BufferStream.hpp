
#pragma once


#include "StreamReader.hpp"
#include "StreamWriter.hpp"

#include <filesystem>

namespace InfinityRenderer {
    class BufferStreamWriter final : public StreamWriter {
    public:
        explicit BufferStreamWriter(Buffer targetBuffer, uint64_t position = 0);

        BufferStreamWriter(const BufferStreamWriter &) = delete;

        ~BufferStreamWriter() override = default;

        [[nodiscard]] bool IsStreamGood() const override { return (bool) m_TargetBuffer; }
        uint64_t GetStreamPosition() override { return m_BufferPosition; }
        void SetStreamPosition(const uint64_t position) override { m_BufferPosition = position; }

        bool WriteData(const char *data, size_t size) override;

        [[nodiscard]] Buffer GetBuffer() const { return {m_TargetBuffer, m_BufferPosition}; }

    private:
        Buffer m_TargetBuffer;
        uint64_t m_BufferPosition = 0;
    };


    class BufferStreamReader final : public StreamReader {
    public:
        explicit BufferStreamReader(Buffer targetBuffer, uint64_t position = 0);

        BufferStreamReader(const BufferStreamReader &) = delete;

        ~BufferStreamReader() override = default;

        [[nodiscard]] bool IsStreamGood() const override { return (bool) m_TargetBuffer; }
        uint64_t GetStreamPosition() override { return m_BufferPosition; }
        void SetStreamPosition(const uint64_t position) override { m_BufferPosition = position; }

        bool ReadData(char *destination, size_t size) override;

        [[nodiscard]] Buffer GetBuffer() const { return {m_TargetBuffer, m_BufferPosition}; }

    private:
        Buffer m_TargetBuffer;
        uint64_t m_BufferPosition = 0;
    };
} // namespace InfinityRenderer
