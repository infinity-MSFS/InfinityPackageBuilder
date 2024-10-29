#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

struct Opacties {
    float start;
    float mid;
    float end;
};

class Menu {
public:
    Menu(ImFont* emojiFont, ImFont* arialFont);

    void ShowMenu(RECT rect, POINT topLeft);

    void drawGradient(RECT rect, POINT topLeft);
    void resetAnimations() {
        m_CurrentHeight = 0.0f;
        m_Opactities = Opacties{
            0.0f,0.0f,0.0f
        };
    }


private:
    void ShowTabContent();

    void DrawButtonCategoryHeader(const char* Categoryname);

    bool DrawCustomButton(const char* icon, const char* label, bool selected);

    int m_CurrentTab;

    float m_CurrentHeight = 0.0f;
    Opacties m_Opactities = Opacties{
        0.0f,0.0f,0.0f
    };

    ImFont* m_emojiFont = nullptr;
    ImFont* m_arialFont = nullptr;
};

void SetThugwareColors(ImGuiStyle& style);



