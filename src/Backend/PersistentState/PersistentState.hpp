
#pragma once

#include "Json/json.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace Infinity {
    struct PersistentStateStruct {
        bool remember_key;
        unsigned int schema_version;

        PersistentStateStruct() : remember_key(false), schema_version(1) {}

        bool operator!=(const PersistentStateStruct &other) const { return remember_key != other.remember_key || schema_version != other.schema_version; }

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(PersistentStateStruct, remember_key, schema_version);
    };
    class PersistentState {
    public:
        static PersistentState *GetInstance() {
            if (!m_Instance) {
                m_Instance = new PersistentState();
            }
            return m_Instance;
        }

        PersistentStateStruct &GetState() { return m_AppState; }

        void SaveState();
        void LoadState();

    private:
        PersistentState() = default;

        static std::string GetFilePath();

    private:
        static PersistentState *m_Instance;
        PersistentStateStruct m_AppState;
        PersistentStateStruct m_PreviousState;
    };
} // namespace Infinity
