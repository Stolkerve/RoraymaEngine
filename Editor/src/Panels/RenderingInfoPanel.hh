#pragma once

#include <RoraymaEngine/RoraymaEngine.hh>
#include <imgui/imgui.h>

namespace rym
{
    class RenderingInfoPanel
    {
    public:
        RenderingInfoPanel()
        {
            values.fill(0.f);
            index = 0;
        }

        void Render()
        {
            auto& statistics = BatchStatistics::Get();

            auto calIndexBecouseThereIsSomeWireError = [&]()
            {
                if (index < 89)
                    index++;
                else
                    index = 0;
            };

            ImGui::Begin(ICON_FA_INFO_CIRCLE"  RenderingInfo");
            ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", statistics.DrawCalls);
            ImGui::Text("Vertices: %d", statistics.NumOfVertices);

            ImGui::End();
        }

        ~RenderingInfoPanel() = default;

    private:
        size_t index;
        std::array<float, 90> values;
    };
}
