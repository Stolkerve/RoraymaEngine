#include "PropertiesPanel.hh"

#include <limits>
#include <fstream>
#include <future>

#include <glm/gtc/type_ptr.hpp>

#include <Portable_File_Dialogs/portable-file-dialogs.h>

#include "CodeEditorPanel.hh"
#include "../CustomImGuiWidgets.hh"
#include "FoldersPanel.hh"


namespace rym
{
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
							auto path = std::filesystem::current_path().string() + "//assets//scripts//" + m_EntitySelected->Tag + ".py";
							std::ofstream file;
							file.open(path);
							file << "import PyScripting\n";
							file << "from PyScripting import Math, Input\n\n";
							file << "#VERY IMPORTANT DO NOT CHANGE THE FILE NAME.\n\n";
							file << "class " << m_EntitySelected->Tag << "(PyScripting.PyScript):\n";
							file << "\ttransform = None\n";
							file << "\tdef OnStart(self):\n\t\tself.transform = self.entity.GetTransformComponent()\n";
							file << "\tdef OnUpdate(self, _delta):\n\t\t#update\n\t\tpass\n";
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
				if (ImGui::Button(ICON_FA_PLUS" Add component", { ImGui::GetContentRegionAvailWidth(), 0}))
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
