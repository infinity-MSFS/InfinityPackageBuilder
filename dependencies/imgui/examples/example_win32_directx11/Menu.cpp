#include "menu.h"
#include "iostream"
#include "imgui_internal.h"

Menu::Menu(ImFont* emojiFont, ImFont* arialFont) : m_emojiFont(emojiFont) , m_arialFont(arialFont), m_CurrentTab(1) {
}


void Menu::ShowMenu(RECT rect , POINT topLeft) {

    int windowWidth = rect.right - rect.left;
    int widowHeight = rect.bottom - rect.top;

    float targetHeight = 920.0f;

    if (m_CurrentHeight < targetHeight) {
        m_CurrentHeight += 50.0f;
        if (m_CurrentHeight > targetHeight) {
            m_CurrentHeight = targetHeight;
        }
    }

    ImVec2 app_window_size = ImVec2(static_cast<float>(windowWidth), static_cast<float>(widowHeight));

    ImGui::SetNextWindowFocus();
    ImGui::SetNextWindowSize(ImVec2(660, m_CurrentHeight), ImGuiCond_Always);
    ImGui::Begin("Thugware", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoTitleBar);


    float rounding = 20.0f;
    ImVec4 left_color = ImVec4(0.1f, 0.1f, 0.1f, 0.6f);
    ImVec4 right_color = ImVec4(0.1f, 0.1f, 0.1f, 0.7f);

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();
    ImVec2 rounded_rect_min_color1 = ImVec2(window_pos.x, window_pos.y);
    ImVec2 rounded_rect_max_color1 = ImVec2(window_pos.x + 200, window_pos.y + window_size.y);
    ImVec2 rounded_rect_min_color2 = ImVec2(window_pos.x + 200, window_pos.y);
    ImVec2 rounded_rect_max_color2 = ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y);

    ImVec2 seperatorLineStart = ImVec2(window_pos.x + 10.0, window_pos.y + 810.0);
    ImVec2 seperatorLineEnd = ImVec2(window_pos.x+190.0, window_pos.y + 810.0);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    draw_list->AddRectFilled(rounded_rect_min_color1, rounded_rect_max_color1, ImColor(left_color), rounding, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft);
    draw_list->AddRectFilled(rounded_rect_min_color2, rounded_rect_max_color2, ImColor(right_color), rounding, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);

    draw_list->AddLine(seperatorLineStart, seperatorLineEnd, ImColor(ImVec4(1.0f, 1.0f, 1.0f, 0.4f)), 2.0f);

    ImGui::BeginChild("Navbar", ImVec2(200, 0),false, ImGuiWindowFlags_NoScrollbar);

    ImGui::SetCursorPos(ImVec2(20.0, 20.0));
    ImGui::PushFont(m_arialFont);
    DrawButtonCategoryHeader("Landing Challenge");
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(10.0, 50.0));

    if (DrawCustomButton("\xE2\x9C\x88", "Landing", m_CurrentTab == 1? true: false) ){
        m_CurrentTab = 1;
    }


    ImGui::SetCursorPos(ImVec2(20.0, 200.0));
    ImGui::PushFont(m_arialFont);
    DrawButtonCategoryHeader("Misc");
    ImGui::PopFont();
    ImGui::SetCursorPos(ImVec2(10.0, 230.0));

    if (DrawCustomButton("\xF0\x9F\x93\x92", "Config", m_CurrentTab == 2 ? true : false)) {
        m_CurrentTab = 2;
    }

    ImGui::SetCursorPos(ImVec2(10.0, 280.0));

    if (DrawCustomButton("\xE2\x9D\x95", "Info", m_CurrentTab == 3 ? true : false)) {
        m_CurrentTab = 3;
    }


    ImGui::SetCursorPos(ImVec2(20.0, 820.0));


    ImGui::PushFont(m_arialFont);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.98f, 0.0f, 0.82f, 1.0f));
    DrawButtonCategoryHeader("Thugware");
    ImGui::PopStyleColor();
    ImGui::PopFont();

    ImGui::SetCursorPos(ImVec2(20.0f, 850.0f));
    ImGui::PushFont(m_emojiFont);
    ImGui::Text("\xF0\x9F\x90\x92");
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::PushFont(m_arialFont);
    ImGui::Text("taco._");
    ImGui::PopFont();


    ImGui::SetCursorPos(ImVec2(20.0f, 880.0f));
    ImGui::PushFont(m_emojiFont);
    ImGui::Text("\xF0\x9F\x94\x91");
    ImGui::PopFont();
    ImGui::SameLine();
    ImGui::PushFont(m_arialFont);
    ImGui::Text("Admin");
    ImGui::PopFont();

    ImGui::EndChild();

    ImGui::SameLine();


    ImGui::BeginChild("RightSide", ImVec2(0, 0), false, ImGuiWindowFlags_NoScrollbar);
    ShowTabContent();
    ImGui::EndChild();
    ImGui::End();
}

void Menu::drawGradient(RECT rect, POINT topLeft) {

    ImGui::SetNextWindowPos(ImVec2((float)topLeft.x, (float)topLeft.y));
    ImGui::SetNextWindowSize(ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top)));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0));

    ImGui::Begin("BackgroundWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoScrollWithMouse);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImVec2 window_pos = ImGui::GetWindowPos();
    ImVec2 window_size = ImGui::GetWindowSize();

    Opacties targetOpacties = Opacties{
        0.0f,0.2f,0.5f
    };

    if (m_Opactities.mid < targetOpacties.mid) {
        m_Opactities.mid += .02f;
    }
    if (m_Opactities.mid > targetOpacties.mid) {
        m_Opactities.mid = targetOpacties.mid;
    }

    if (m_Opactities.end < targetOpacties.end) {
        m_Opactities.end += .05f;
    }
    if (m_Opactities.end > targetOpacties.end) {
        m_Opactities.end = targetOpacties.end;
    }

    ImU32 col_top = ImGui::GetColorU32(ImVec4(0.98f, 0.0f, 0.82f, 0.00f));
    ImU32 col_middle = ImGui::GetColorU32(ImVec4(0.98f, 0.0f, 0.82f, m_Opactities.mid));
    ImU32 col_bottom = ImGui::GetColorU32(ImVec4(0.98f, 0.0f, 0.82f, m_Opactities.end));


    float gradient_height = window_size.y * 0.75f; 
    draw_list->AddRectFilledMultiColor(
        window_pos, ImVec2(window_pos.x + window_size.x, window_pos.y + gradient_height),
        col_top, col_top, col_middle, col_middle);

    draw_list->AddRectFilledMultiColor(
        ImVec2(window_pos.x, window_pos.y + gradient_height), ImVec2(window_pos.x + window_size.x, window_pos.y + window_size.y),
        col_middle, col_middle, col_bottom, col_bottom);

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);

}



void Menu::ShowTabContent() {
    switch (m_CurrentTab) {
    case 1:
        ImGui::PushFont(m_arialFont);
        ImGui::Text("Teleport");
        ImGui::PopFont();
     
       
        break;
    case 2:
        ImGui::PushFont(m_arialFont);
        ImGui::Text("Config");
        ImGui::PopFont();
      
        break;
    case 3:
        ImGui::PushFont(m_arialFont);
        ImGui::Text("About");
        ImGui::PopFont();
        break;

    default:
        ImGui::Text("Please select a tab.");
        break;
    }
}

void Menu::DrawButtonCategoryHeader(const char* Categoryname) {
    ImGui::Text(Categoryname);
}

bool Menu::DrawCustomButton(const char* icon, const char* label, bool selected)
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window == nullptr)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const float label1Size = ImGui::CalcTextSize(icon).x *2;
    const float label2Size = ImGui::CalcTextSize(label).x *2;
    const ImVec2 padding = ImVec2(5.0f, 5.0f);
    const ImVec2 size = ImVec2(170.0, 40.0);

    const ImVec2 pos = window->DC.CursorPos;

    const ImU32 textColor =  ImGui::GetColorU32(ImGuiCol_Text);
     ImU32 bgColor = NULL;
    if (selected) {
        bgColor = ImGui::GetColorU32(ImGuiCol_ButtonActive);
    }
    else {
        bgColor = ImGui::GetColorU32(ImGuiCol_Button);
    }
     

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 bottomRightPos = ImVec2(pos.x + size.x, pos.y + size.y);
    drawList->AddRectFilled(pos, bottomRightPos, bgColor, style.FrameRounding);

    ImVec2 textPos = ImVec2(pos.x + 10.0 , pos.y + 12.0f);
    ImVec2 centerPos = ImVec2(pos.x + size.x / 2, pos.y + size.y / 2);
    float halfLabelWidth = (label1Size + label2Size) / 2.0f;
   
    ImGui::PushFont(m_emojiFont);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.98f, 0.0f, 0.82f, 1.0f));
    drawList->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), icon);
    ImGui::PopStyleColor();
    ImGui::PopFont();

    textPos.x += 45.0;
    textPos.y -= 2.0;
    ImGui::PushFont(m_arialFont);
    drawList->AddText(textPos, textColor, label);
    ImGui::PopFont();

    const ImVec2 mousePos = ImGui::GetIO().MousePos;
    const bool hovered = mousePos.x >= pos.x && mousePos.y >= pos.y &&
        mousePos.x <= bottomRightPos.x && mousePos.y <= bottomRightPos.y;
    ImGui::PopStyleColor(2);

    if (hovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
      
        return true;
    }
    
    return false;
}

void SetThugwareColors(ImGuiStyle& style)
{


    style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f); 
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);
}
