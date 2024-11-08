
#include "BufferStream.hpp"

namespace InfinityRenderer {
    BufferStreamWriter::BufferStreamWriter(const Buffer targetBuffer, const uint64_t position) :
        m_TargetBuffer(targetBuffer), m_BufferPosition(position) {}

    bool BufferStreamWriter::WriteData(const char *data, const size_t size) {

        if (const bool valid = m_BufferPosition + size <= m_TargetBuffer.Size; !valid)
            return false;

        memcpy(m_TargetBuffer.As<uint8_t>() + m_BufferPosition, data, size);
        m_BufferPosition += size;
        return true;
    }

    BufferStreamReader::BufferStreamReader(const Buffer targetBuffer, const uint64_t position) :
        m_TargetBuffer(targetBuffer), m_BufferPosition(position) {}

    bool BufferStreamReader::ReadData(char *destination, const size_t size) {

        if (const bool valid = m_BufferPosition + size <= m_TargetBuffer.Size; !valid)
            return false;

        memcpy(destination, m_TargetBuffer.As<uint8_t>() + m_BufferPosition, size);
        m_BufferPosition += size;
        return true;
    }
} // namespace InfinityRenderer
