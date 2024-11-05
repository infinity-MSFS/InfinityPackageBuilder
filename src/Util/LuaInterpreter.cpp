
#include "LuaInterpreter.hpp"
#include <fstream>


namespace InfinityPackageBuilder::Lua {


    LuaInterpreter::LuaInterpreter(const char *file_path) : m_LuaError({-1, ""}), m_FilePath(file_path), m_Language(TextEditor::LanguageDefinition::Lua()) {
        m_TextEditor.SetLanguageDefinition(m_Language);
        m_TextEditor.SetPalette(TextEditor::GetDarkPalette());
        m_Lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::math, sol::lib::io, sol::lib::os, sol::lib::utf8);


        if (std::ifstream t(m_FilePath); t.good()) {
            const std::string str((std::istreambuf_iterator(t)), std::istreambuf_iterator<char>());
            m_TextEditor.SetText(str);
        }
    }


    void LuaInterpreter::Save() const {
        const auto text = m_TextEditor.GetText();

        if (std::ofstream out(m_FilePath); out.good()) {
            out << text;
        }
    }


    void LuaInterpreter::RenderCodebox() { m_TextEditor.Render("Lua Editor"); }

    void LuaInterpreter::InsertError(int line, std::string &error) {
        m_ErrorMarkers.insert(std::pair(line, error));
        m_TextEditor.SetErrorMarkers(m_ErrorMarkers);
    }

    void LuaInterpreter::ClearErrors() {
        m_LuaError = {-1, ""};
        m_ErrorMarkers.clear();
        m_TextEditor.SetErrorMarkers(m_ErrorMarkers);
    }


    void LuaInterpreter::RunLua() {
        const std::string code = m_TextEditor.GetText();
        try {
            ClearErrors();
            m_Lua.script(code);
        } catch (const sol::error &e) {

            if (const auto error = LuaError::CreateLuaError(e.what()); error.has_value()) {
                m_LuaError = static_cast<LuaError>(*error);
                InsertError(m_LuaError.line_number, m_LuaError.error);
            }
        }
    }

    LuaError &LuaInterpreter::GetLuaError() { return m_LuaError; }


    void LuaInterpreter::SetIdentifiers() {
        for (const auto &idenifier: m_FunctionNames) {
            TextEditor::Identifier id;
            id.mDeclaration = idenifier.second;
            m_Language.mIdentifiers.insert(std::make_pair(idenifier.first, id));

            m_TextEditor.SetLanguageDefinition(m_Language);
        }
    }


} // namespace InfinityPackageBuilder::Lua
