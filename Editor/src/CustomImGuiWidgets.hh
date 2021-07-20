#pragma once

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <RoraymaEngine/Scene/Entity.hh>
#include <RoraymaEngine/Core/ImGui/IconsFontAwesome5.h>

#include <string_view>
#include <string>

namespace rym {
	void CreateModalOkCancel(const char* name, const char* messege, bool* ok, bool* cancel, ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize);

	void CreateModalOk(const char* name, const char* messege, bool* ok, ImGuiWindowFlags flags = ImGuiWindowFlags_AlwaysAutoResize);

	void DragMouseDisable();

	template<typename T>
	bool DeleteComponent(const std::shared_ptr<Entity>& entitySelected)
	{
		bool res = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(ICON_FA_TRASH"  Delete component"))
			{
				res = entitySelected->DeleteComponent<T>();
			}
			ImGui::EndPopup();
		}
		return res;
	}

	bool DrawDragIntPlusMinus(const char* label, int* value, int reset = 0, int min = 0, int max = 0);

	bool DrawDragFloat(const char* label, float* value, float force, float min = 0.f, float max = 0.f);

	bool DrawDragFloat2(const char* label, float* value, float force);

	bool DrawVec2(const std::string_view& label, glm::vec2& vec, float force = 1.0f, float resetValue = 0.f);

	bool DrawDragFloat(const std::string& label, float& value, float force = 1.f, float reset = 0.f, float min = 0.0f, float max = 0.0f);

	bool DrawSliderFloat(const std::string& label, float& value, float reset, float min, float max);

	bool DrawSliderInt(const std::string& label, int& value, int reset, int min, int max);

	static void HelpMarker(const char* desc);

	void DrawTextureButton(SpriteComponent* sprite, TransformComponent* transform);

	bool DrawCheckBox(const char* name, bool* b);

	void DrawColorPicker4(const char* name, float* col);

	std::pair<bool, std::string> DrawTextInput(const char* name, std::string& target, int max);
}