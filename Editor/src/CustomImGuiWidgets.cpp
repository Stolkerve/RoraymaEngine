#include "CustomImGuiWidgets.hh"
#include "Panels/FoldersPanel.hh"

#include <imgui/imgui_internal.h>

#include <RoraymaEngine/Core/Input.hh>
#include <RoraymaEngine/Core/ImGui/IconsFontAwesome5.h>
#include <RoraymaEngine/Scene/Components.hh>
#include <RoraymaEngine/Scene/AssetsManager.hh>


namespace rym {
	void CreateModalOkCancel(const char* name, const char* messege, bool* ok, bool* cancel, ImGuiWindowFlags flags)
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

	void CreateModalOk(const char* name, const char* messege, bool* ok, ImGuiWindowFlags flags)
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
	constexpr int sameLineSpace = 72;

	void DragMouseDisable()
	{
		//RYM_INFO(ImGui::IsMouseDragging(0));
		if (ImGui::IsItemActivated() && !ImGui::IsMouseDoubleClicked(0))
			Input::SetCursorMode(CursorMode::DISABLED);
		if (ImGui::IsItemDeactivated())
			Input::SetCursorMode(CursorMode::NORMAL);
	}


	bool DrawDragIntPlusMinus(const char* label, int* value, int reset, int min, int max)
	{
		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::SameLine(sameLineSpace);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		if (ImGui::Button(ICON_FA_REDO))
			*value = reset;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		const bool open = ImGui::DragInt(label, value, 1, min, max);
		ImGui::PopStyleVar();
		ImGui::PopID();

		return open;
	}

	bool DrawDragFloat(const char* label, float* value, float force, float min, float max)
	{
		const bool open = ImGui::DragFloat(label, value, force, min, max);
		DragMouseDisable();
		return open;
	}

	bool DrawDragFloat2(const char* label, float* value, float force)
	{
		const bool open = ImGui::DragFloat2(label, value, force);
		DragMouseDisable();
		return open;
	}

	bool DrawVec2(const std::string_view& label, glm::vec2& vec, float force, float resetValue, float space)
	{
		ImGui::PushID(label.data());
		ImGui::Text(label.data());
		ImGui::SameLine(space);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		if (ImGui::Button(ICON_FA_REDO))
			vec = { resetValue, resetValue };
		ImGui::SameLine();
		ImGui::PushID(0);
		auto ptrToVec = glm::value_ptr(vec);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		const bool open = DrawDragFloat2("##label", glm::value_ptr(vec), force);
		ImGui::PopID();
		ImGui::PopStyleVar();
		ImGui::PopID();
		return open;
	}

	bool DrawDragFloat(const std::string& label, float& value, float force, float reset, float min, float max)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text(label.c_str());
		ImGui::SameLine(sameLineSpace);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		if (ImGui::Button(ICON_FA_REDO))
			value = reset;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		const bool open = DrawDragFloat("##label", &value, force, min, max);
		ImGui::PopStyleVar();
		ImGui::PopID();

		return open;
	}

	bool DrawSliderFloat(const std::string& label, float& value, float reset, float min, float max)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text(label.c_str());
		ImGui::SameLine(sameLineSpace);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		if (ImGui::Button(ICON_FA_REDO))
			value = reset;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		const bool open = ImGui::SliderFloat("##label", &value, max, min);
		ImGui::PopStyleVar();
		ImGui::PopID();

		return open;
	}

	bool DrawSliderInt(const std::string& label, int& value, int reset, int min, int max)
	{
		ImGui::PushID(label.c_str());
		ImGui::Text(label.c_str());
		ImGui::SameLine(sameLineSpace);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		if (ImGui::Button(ICON_FA_REDO))
			value = reset;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		const bool open = ImGui::SliderInt("##label", &value, max, min);
		ImGui::PopStyleVar();
		ImGui::PopID();

		return open;
	}

	static void HelpMarker(const char* desc)
	{
		ImGui::SameLine();
		ImGui::TextDisabled(" (?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void DrawTextureButton(SpriteComponent* sprite, TransformComponent* transform)
	{
		/*
		auto searchImage = [&]() {
			auto workSpacePath = std::filesystem::current_path();
			auto lastPath = pfd::open_file("Textures files", workSpacePath.string(),
				{ "Images Files (.jpeg .png .bmp .jpg)", "*.jpeg *.png *.bmp *.jpg" }).result();
			std::filesystem::path path = lastPath.back();
			if (!lastPath.empty())
				AssetsManager::PushTexture(path.string(), path.filename().stem().string());
		};
		*/

		ImGui::Text("Texture");

		ImGui::SameLine(sameLineSpace);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		const auto& texture = AssetsManager::GetTexture(sprite->texture);
		if (texture.get())
		{
			ImGui::Image(reinterpret_cast<void*>(texture->GetID()), { 80, 80 }, { 0, 1 }, { 1, 0 });
			/*
			if (ImGui::IsItemClicked())
				searchImage();
			*/
		}
		else
		{
			ImGui::Button("Drop texture");
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FOLDDERS_PANEL"))
				{
					const auto castedData = reinterpret_cast<const FoldersData*>(payload->Data);
					const auto ex = castedData->Path.extension();
					if (ex == ".jpeg" || ex == ".png" || ex == ".bmp" || ex == ".jpg")
					{
						const auto name = castedData->Path.stem().string();
						const auto path = castedData->Path.string();
						sprite->texture = name;
						auto tex = AssetsManager::PushTexture(name, path);
						float textureWidth = static_cast<float>(tex->GetWidth());
						float textureHeight = static_cast<float>(tex->GetHeight());
						transform->scale = { textureWidth, textureHeight };
						sprite->size = { textureWidth,textureHeight };
					}
				}
				ImGui::EndDragDropTarget();
			}
			//if (ImGui::IsItemClicked())
			//	searchImage();
		}
		ImGui::PopStyleVar();
	}

	bool DrawCheckBox(const char* name, bool* b)
	{
		ImGui::PushID(name);
		ImGui::Text(name);

		ImGui::SameLine(sameLineSpace);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		const bool is = ImGui::Checkbox("##", b);
		ImGui::PopStyleVar();
		ImGui::PopID();
		return is;
	}

	void DrawColorPicker4(const char* name, float* col)
	{
		ImGui::PushID(name);
		ImGui::Text(name);
		ImGui::SameLine(sameLineSpace);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImGui::ColorEdit4("##", col, ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_NoLabel);
		ImGui::PopID();
	}

	std::pair<bool, std::string> DrawTextInput(const char* name, std::string& target, int max)
	{
		ImGui::PushID(name);
		ImGui::Text("Name ");
		ImGui::SameLine(sameLineSpace);
		char buffer[30];
		std::strcpy(buffer, target.c_str());
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool res = ImGui::InputText("###name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank);
		ImGui::PopID();
		return std::make_pair(res, buffer);
	}

	void DrawArrayVec2(std::vector<glm::vec2>& vectors)
	{
		size_t i = 0;
		for (auto& v : vectors)
		{
			DrawVec2(std::to_string(i), v, 1.f, 0.f, 40.f);
			if (ImGui::BeginPopupContextItem(std::to_string(i).c_str()))
			{
				if (ImGui::MenuItem("delete point"))
				{
					vectors.erase(vectors.begin() + i);
				}
				ImGui::EndPopup();
			}
			i++;
		}
		ImGui::Spacing();
		ImGui::SetCursorPosX(40.f);
		if (ImGui::Button(ICON_FA_PLUS" Add point"))//{ ImGui::GetContentRegionAvailWidth(), 0 }
		{
			vectors.push_back(glm::vec2(0.f, 0.f));
		}
	}

	struct InputTextCallback_UserData
	{
		std::string* Str;
		ImGuiInputTextCallback  ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
			std::string* str = user_data->Str;
			IM_ASSERT(data->Buf == str->c_str());
			str->resize(data->BufTextLen);
			data->Buf = (char*)str->c_str();
		}
		else if (user_data->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = user_data->ChainCallbackUserData;
			return user_data->ChainCallback(data);
		}
		return 0;
	}

	bool StdInputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}

	bool StdInputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
	}

	bool StdInputTextWithHint(const char* label, const char* hint, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
	{
		IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
		flags |= ImGuiInputTextFlags_CallbackResize;

		InputTextCallback_UserData cb_user_data;
		cb_user_data.Str = str;
		cb_user_data.ChainCallback = callback;
		cb_user_data.ChainCallbackUserData = user_data;
		return ImGui::InputTextWithHint(label, hint, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
	}
}