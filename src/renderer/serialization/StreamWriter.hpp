
#pragma once

#include "renderer/buffer/Buffer.hpp"

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace InfinityRenderer {
    class StreamWriter {
    public:
        virtual ~StreamWriter() = default;

        [[nodiscard]] virtual bool IsStreamGood() const = 0;

        virtual uint64_t GetStreamPosition() = 0;

        virtual void SetStreamPosition(uint64_t position) = 0;

        virtual bool WriteData(const char *data, size_t size) = 0;

        explicit operator bool() const { return IsStreamGood(); }

        void WriteBuffer(Buffer buffer, bool writeSize = true);

        void WriteZero(uint64_t size);

        void WriteString(const std::string &string);

        void WriteString(std::string_view string);

        template<typename T>
        void WriteRaw(const T &type) {
            WriteData((char *) &type, sizeof(T));
        }

        template<typename T>
        void WriteObject(const T &obj) {
            T::Serialize(this, obj);
        }

        template<typename Key, typename Value>
        void WriteMap(const std::map<Key, Value> &map, const bool writeSize = true) {
            if (writeSize)
                WriteRaw<uint32_t>(static_cast<uint32_t>(map.size()));

            for (const auto &[key, value]: map) {
                if constexpr (std::is_trivial<Key>())
                    WriteRaw<Key>(key);
                else
                    WriteObject<Key>(key);

                if constexpr (std::is_trivial<Value>())
                    WriteRaw<Value>(value);
                else
                    WriteObject<Value>(value);
            }
        }

        template<typename Key, typename Value>
        void WriteMap(const std::unordered_map<Key, Value> &map, const bool writeSize = true) {
            if (writeSize)
                WriteRaw<uint32_t>(static_cast<uint32_t>(map.size()));

            for (const auto &[key, value]: map) {
                if constexpr (std::is_trivial<Key>())
                    WriteRaw<Key>(key);
                else
                    WriteObject<Key>(key);

                if constexpr (std::is_trivial<Value>())
                    WriteRaw<Value>(value);
                else
                    WriteObject<Value>(value);
            }
        }


        template<typename Value>
        void WriteMap(const std::unordered_map<std::string, Value> &map, const bool writeSize = true) {
            if (writeSize)
                WriteRaw<uint32_t>(static_cast<uint32_t>(map.size()));

            for (const auto &[key, value]: map) {
                WriteString(key);

                if constexpr (std::is_trivial<Value>())
                    WriteRaw<Value>(value);
                else
                    WriteObject<Value>(value);
            }
        }

        template<typename T>
        void WriteArray(const std::vector<T> &array, const bool writeSize = true) {
            if (writeSize)
                WriteRaw<uint32_t>(static_cast<uint32_t>(array.size()));

            for (const auto &element: array) {
                if constexpr (std::is_trivial<T>())
                    WriteRaw<T>(element);
                else
                    WriteObject<T>(element);
            }
        }
#ifdef _MSC_VER
        template<> // Only MSVC supports empty templates
#endif
        void WriteArray(const std::vector<std::string> &array, const bool writeSize) {
            if (writeSize)
                WriteRaw<uint32_t>(static_cast<uint32_t>(array.size()));

            for (const auto &element: array)
                WriteString(element);
        }
    };
} // namespace InfinityRenderer
