
#pragma once

#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include "TextEditor.h"
#include "sol/sol.hpp"

namespace InfinityPackageBuilder::Lua {

    struct LuaError {
        int line_number{};
        std::string error;

        static std::optional<LuaError> CreateLuaError(const char *raw_error) {
            const std::regex error_regex(R"(:\s*(\d+)\s*:)");
            const std::string raw_error_string(raw_error);

            std::cout << "Parsing String " << raw_error_string << std::endl;

            if (std::smatch match; std::regex_search(raw_error_string, match, error_regex) && !match.empty()) {
                const std::string line_number = match[1].str();

                return LuaError{std::stoi(line_number), raw_error_string};
            }

            return std::nullopt;
        }
    };

    class LuaInterpreter {
    public:
        explicit LuaInterpreter(const char *file_path);


        template<typename Func>
        void AddFunctionToLua(const std::string &name, Func &&function, std::string &signature) {
            m_Lua.set_function(name, std::forward<Func>(function));
            m_FunctionNames.emplace_back(name, signature);
            SetIdentifiers();
        }
        template<typename Func>
        void AddFunctionToLua(const std::vector<std::tuple<std::string &, Func &&, std::string &>> fn_vec) {
            for (const auto function: fn_vec) {
                AddFunctionToLua(function[0], function[1], function[2]);
            }
        }

        void RenderCodebox();

        void RunLua();

        LuaError &GetLuaError();

        void Paste() { m_TextEditor.Paste(); }

        void Save() const;

    private:
        void InsertError(int line, std::string &error);

        void ClearErrors();

        void SetIdentifiers();

    private:
        TextEditor m_TextEditor;
        TextEditor::ErrorMarkers m_ErrorMarkers;
        sol::state m_Lua;
        LuaError m_LuaError;
        const char *m_FilePath;
        std::vector<std::pair<std::string, std::string>> m_FunctionNames;
        TextEditor::LanguageDefinition m_Language;
    };

} // namespace InfinityPackageBuilder::Lua
