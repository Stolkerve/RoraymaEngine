#include "PropertiesPanel.hh"

#include <limits>
#include <fstream>
#include <future>

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>
#include <Portable_File_Dialogs/portable-file-dialogs.h>

#include "FoldersPanel.hh"
#include "CodeEditorPanel.hh"

namespace rym
{
	constexpr int sameLineSpace = 72;

	void DragMouseDisable()
	{
		//RYM_INFO(ImGui::IsMouseDragging(0));
		if (ImGui::IsItemActivated() && !ImGui::IsMouseDoubleClicked(0))
			Input::SetCursorMode(CursorMode::DISABLED);
		if (ImGui::IsItemDeactivated())
			Input::SetCursorMode(CursorMode::NORMAL);
	}

	template<typename T>
	void DeleteComponent(const std::shared_ptr<Entity>& entitySelected)
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(ICON_FA_TRASH"  Delete component"))
			{
				entitySelected->DeleteComponent<T>();
			}
			ImGui::EndPopup();
		}
	}

	bool DrawDragIntPlusMinus(const char* label, int* value, int reset = 0, float min = 0.f, float max = 0.f)
	{
		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::SameLine(sameLineSpace);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, GImGui->Style.FramePadding.y });
		if (ImGui::Button(ICON_FA_REDO))
			*value = reset;
		ImGui::SameLine();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		const bool open = ImGui::DragInt(label, value, 1, min, max);
		ImGui::PopStyleVar();
		ImGui::PopID();

		return open;
	}

	bool DrawDragFloat(const char* label, float* value, float force, float min = 0.f, float max = 0.f)
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

	bool DrawVec2(const std::string_view& label, glm::vec2& vec, float force = 1.0f, float resetValue = 0.f)
	{
		ImGui::PushID(label.data());
		ImGui::Text(label.data());
		ImGui::SameLine(sameLineSpace);
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

	bool DrawDragFloat(const std::string& label, float& value, float force = 1.f, float reset = 0.f, float min = 0.0f, float max = 0.0f)
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

	void DrawTextureButton(SpriteComponent* sprite)
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
						const auto name = castedData->Path.filename().stem().string();
						const auto path = castedData->Path.string();
						AssetsManager::PushTexture(name, path);
						sprite->texture = name;
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

	void PropertiesPanel::Render()
	{
		//ImGui::SetNextWindowSizeConstraints({ 50, std::numeric_limits<float>().min() }, { 430, std::numeric_limits<float>().min() });
		ImGui::Begin(ICON_FA_USER_SECRET"  PropertiesPanel");

		ImGuiTreeNodeFlags headerFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;

		if (m_EntitySelected != nullptr)
		{
			/*** TAG ***/
			if (ImGui::CollapsingHeader(ICON_FA_TAG"  Tag", headerFlags))
			{
				auto [applied, res] = DrawTextInput("Tag", m_EntitySelected->Tag, 30);
				if(applied)
					m_EntitySelected->Tag = res;
			}

			/*** Transform ***/
			if (m_EntitySelected->HaveComponent<TransformComponent>())
			{
				ImGui::Separator();

				auto transfrom = m_EntitySelected->GetComponent<TransformComponent>();

				if (ImGui::CollapsingHeader(ICON_FA_EXPAND_ARROWS_ALT"  Transfrom", headerFlags))
				{
					static bool changed = false;
					//RYM_INFO("{0}. Antes de referenciar", transfrom->Translation);
					DrawVec2("Position", transfrom->translation, 0.1f);
					//RYM_INFO("{0}. Despues de referenciar", transfrom->Translation);
					DrawVec2("Scale", transfrom->scale, 0.1f, 1.f);
					DrawSliderFloat("Rotation", transfrom->rotation, 0.f, -360.f, 360.f);
				}
			}

			/*** Camera ***/
			if (m_EntitySelected->HaveComponent<CameraComponent>())
			{
				ImGui::Separator();

				auto camera = m_EntitySelected->GetComponent<CameraComponent>();
				if (ImGui::CollapsingHeader(ICON_FA_VIDEO"  Camera", headerFlags))
				{
					DeleteComponent<CameraComponent>(m_EntitySelected);
					DrawCheckBox("Current", &camera->current);
					//float preOrthoSize = camera->Camera.GetOrthoSize();
					//float orthoSize = preOrthoSize;
					if (DrawDragFloat("OrthoSize", camera->camera.m_OrthoSize, 1.f, 500.f))
						camera->camera.SetOrthoSize(camera->camera.m_OrthoSize);
				}
			}

			/*** Sprite ***/
			if (m_EntitySelected->HaveComponent<SpriteComponent>())
			{
				ImGui::Separator();

				auto sprite = m_EntitySelected->GetComponent<SpriteComponent>();
				auto transfrom = m_EntitySelected->GetComponent<TransformComponent>();

				if (ImGui::CollapsingHeader(ICON_FA_GHOST"  Sprite", headerFlags))
				{
					DeleteComponent<SpriteComponent>(m_EntitySelected);
					DrawTextureButton(sprite);

					if (DrawCheckBox("FlipH", &sprite->flipH))
						transfrom->scale.x = sprite->flipH ? transfrom->scale.x * -1.f : transfrom->scale.x * -1.f;
					if (DrawCheckBox("FlipV", &sprite->flipV))
						transfrom->scale.y = sprite->flipV ? transfrom->scale.y * -1.f : transfrom->scale.y * -1.f;

					//ImGui::Separator();
					// Color treenode
					DrawColorPicker4("Self color", glm::value_ptr(sprite->color.rgba));
					int a = 0;
					DrawSliderInt("Layer", sprite->layer, 0, -20, 20);
				}
			}

			if (m_EntitySelected->HaveComponent<PyScriptComponent>())
			{
				ImGui::Separator();
				auto script = m_EntitySelected->GetComponent<PyScriptComponent>();
				if (ImGui::CollapsingHeader(ICON_FA_CODE"  PyScripts", headerFlags))
				{
					DeleteComponent<PyScriptComponent>(m_EntitySelected);
					if (script->filePath.empty())
					{
						if (ImGui::Button("Create"))
						{
							auto path = std::filesystem::current_path().string() + "//scripts//" + m_EntitySelected->Tag + ".py";
							std::ofstream file;
							file.open(path);
							file << "import PyScripting\n";
							file << "from PyScripting import Math, Input\n\n";
							file << "#VERY IMPORTANT DO NOT CHANGE THE FILE NAME.\n\n";
							file << "class " << m_EntitySelected->Tag << "(PyScripting.PyScript):\n";
							file << "\tdef OnStart(self):\n\t\tpass\n";
							file << "\tdef OnUpdate(self, _delta):\n\t\t#transform = self.entity.GetTransformComponent()\n\t\tpass\n";
							file << "\tdef OnQuit(self):\n\t\tpass\n";
							file.close();
							script->filePath = path;
							script->moduleName = m_EntitySelected->Tag;
							script->CreateScript(std::make_shared<PyScript>(script->moduleName, m_EntitySelected));

							CodeEditorPanel::PushFile(path);
						}
						ImGui::SameLine();
						ImGui::Button("Drop");
						if (ImGui::BeginDragDropTarget())
						{
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FOLDDERS_PANEL"))
							{
								auto castedData = reinterpret_cast<const FoldersData*>(payload->Data);
								auto ex = castedData->Path.extension();
								if (ex == ".py")
								{
									script->filePath = castedData->Path.string();
									script->moduleName = castedData->Path.stem().string();
									script->CreateScript(std::make_shared<PyScript>(script->moduleName, m_EntitySelected));
									CodeEditorPanel::PushFile(script->filePath);
								}
							}
							ImGui::EndDragDropTarget();
						}
					}
					else
					{
						ImGui::Button((m_EntitySelected->Tag + ".py").c_str());
					}
				}
			}

			/* Add component */
			{
				ImGui::Separator();
				if (ImGui::Button(ICON_FA_PLUS" Add component"))
				{
					ImGui::OpenPopup("Components_list");
				}
				if(ImGui::BeginPopup("Components_list"))
				{
					if (ImGui::Selectable("Sprite Component"))
					{
						m_EntitySelected->AddComponent<SpriteComponent>();
					}
					if (ImGui::Selectable("Camera Component"))
					{
						m_EntitySelected->AddComponent<CameraComponent>(m_CurrentScene->m_Size.x / m_CurrentScene->m_Size.y);
					}
					if (ImGui::Selectable("PyScript Component"))
					{
						m_EntitySelected->AddComponent<PyScriptComponent>();
					}
					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();
	}
}
