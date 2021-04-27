#include "ConsolePanel.hh"
#include <RoraymaEngine/RoraymaEngine.hh>
#include <imgui/imgui.h>
#include <vector>

namespace rym
{
    ConsolePanel* ConsolePanel::s_Instace = nullptr;

    ConsolePanel* ConsolePanel::GetInstace()
    {
        if (!s_Instace)
            s_Instace = new ConsolePanel;
        return s_Instace;
    }

    void ConsolePanel::PushMessage(const std::string_view& str, mLevel level)
    {
        //ImGui::PushStyleColor(ImGuiCol_Text, {0.f, 1.f, 0.f, 1.f});
        m_Buffer.append(str);
        m_Buffer.append("\n");
        //ImGui::PopStyleColor();
        m_StyleCount++;
    }

    void ConsolePanel::PushString(const std::string_view& str)
    {
        m_Buffer.append(str);
        m_Buffer.append("\n");
    }

    void ConsolePanel::Render()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin(ICON_FA_TERMINAL"  Console Debug");
        ImGui::InputTextMultiline("##Console", (char*)m_Buffer.c_str(), m_Buffer.size(), ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 20),
            ImGuiInputTextFlags_ReadOnly);
        ImGui::End();
        ImGui::PopStyleVar();
        //ImGui::PopStyleColor(m_StyleCount);
    }

    void ConsolePanel::PushCommand(const std::string_view& str)
    {
    }
}