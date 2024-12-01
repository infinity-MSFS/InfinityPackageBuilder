
#include "LauncherJsonManager.hpp"

#include "Backend/Application/Application.hpp"

namespace Infinity {
    LauncherJsonManager::LauncherJsonManager(const float padding_x, const float padding_y) : Page(padding_x, padding_y), m_FileSelector(), m_ValidationType(ValidationTypes::NOT_AUTHORIZED), m_GroupKeys(Key::GetKeyMap()) {}

    void LauncherJsonManager::RenderPage() {
        const auto word_size = ImGui::CalcTextSize("Launcher Json Manager");
        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - word_size.x / 2, 10.0f));
        ImGui::Text("Launcher Json Manager");
        m_FileSelector.Render();
        if (ImGui::Button("Fatal")) {
            Error(ErrorType::Fatal, "Fatal Error").Dispatch();
        }
        if (ImGui::Button("Non-Fatal")) {
            Error(ErrorType::NonFatal, "Non-Fatal Error").Dispatch();
        }
        if (ImGui::Button("Warning")) {
            Error(ErrorType::Warning, "Warning").Dispatch();
        }

        if (m_FileSelector.GetKey().has_value() && m_FileSelector.GetKey().value().size() == 32 && m_ValidationType == ValidationTypes::NOT_AUTHORIZED) {
            const auto key = m_FileSelector.GetKey().value();
            m_ValidationType = Key::GetValidationType({key, Keys::Unknown}, m_GroupKeys);
        }else {
            switch (m_ValidationType) {
                case ValidationTypes::FULL: {
                    ImGui::Text("Full");
                    break;
                }
                case ValidationTypes::AERO_DYNAMICS: {
                    ImGui::Text("Aero Dynamics");
                    break;
                }
                case ValidationTypes::DELTA_SIM: {
                    ImGui::Text("Delta Simulation");
                    break;
                }
                case ValidationTypes::LUNAR_SIM: {
                    ImGui::Text("Lunar Simulation");
                    break;
                }
                case ValidationTypes::OUROBOROS: {
                    ImGui::Text("Ouroboros");
                    break;
                }
                case ValidationTypes::QBIT_SIM: {
                    ImGui::Text("Qbit Simulation");
                    break;
                }
                case ValidationTypes::NOT_AUTHORIZED:
                    default: {
                    ImGui::Text("Unknown / Not Authorized");
                    break;
                    }

            }
        }
    }
} // namespace Infinity
