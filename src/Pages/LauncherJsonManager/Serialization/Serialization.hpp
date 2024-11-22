
#pragma once
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "BinaryStructure.hpp"


class Serialization {
public:
    static Serialization &GetInstance() {
        static Serialization instance;
        return instance;
    }

    static std::vector<uint8_t> SerializeAndCompress(const SerializationTypes::Outgoing::OutgoingBinary &binary);
    static std::map<std::string, SerializationTypes::Incoming::IncomingBinary> DecompressAndDeserialize(std::vector<uint8_t> &binary);
    static void WriteToFile(const std::string &file_name, const std::vector<uint8_t> &data);

    static std::optional<std::map<std::string, SerializationTypes::Incoming::IncomingBinary>> GetGroupData() {
        if (!m_GroupMap.empty())
            return m_GroupMap;
        return std::nullopt;
    }

private:
    Serialization() = default;
    ~Serialization() = default;

    Serialization(const Serialization &) = delete;
    Serialization &operator=(const Serialization &) = delete;

private:
    static std::map<std::string, SerializationTypes::Incoming::IncomingBinary> m_GroupMap;
};
