#pragma once

#include <imgui/imgui.h>

namespace rym
{
	void CreateModalOkCancel(const char* name, const char* messege, bool* ok, bool* cancel, ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize)
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal(name, NULL, flags))
		{
			//ImGui::Text("All thoses files going to be move to the recicle bin\n\n");
			ImGui::Text(messege);
			ImGui::Separator();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			ImGui::PopStyleVar();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				*ok = true;
				ImGui::CloseCurrentPopup();
			}

			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				*cancel = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void CreateModalOk(const char* name, const char* messege, bool* ok, ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize)
	{
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal(name, NULL, flags))
		{
			ImGui::Text(messege);
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				*ok = true;
				ImGui::CloseCurrentPopup();
			}

			//ImGui::SetItemDefaultFocus();
			ImGui::EndPopup();
		}
	}

}