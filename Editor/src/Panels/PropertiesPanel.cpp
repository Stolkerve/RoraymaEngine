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

		if (m_EntitySelected.entity)
		{
			/*** TAG ***/
			if (ImGui::CollapsingHeader(ICON_FA_TAG"  Tag", headerFlags))
			{
				auto [applied, res] = DrawTextInput("Tag", m_EntitySelected.entity->Tag, 30);
				if(applied)
					m_EntitySelected.entity->Tag = res;
			}

			/*** Transform ***/
			if (m_EntitySelected.transformComponent)
			{
				ImGui::Separator();


				if (ImGui::CollapsingHeader(ICON_FA_EXPAND_ARROWS_ALT"  Transfrom", headerFlags))
				{
					static bool changed = false;
					//RYM_INFO("{0}. Antes de referenciar", transfrom->Translation);
					DrawVec2("Position", m_EntitySelected.transformComponent->translation, 0.1f);
					//RYM_INFO("{0}. Despues de referenciar", transfrom->Translation);
					if (m_EntitySelected.spriteComponent)
					{
						if (!m_EntitySelected.spriteComponent->texture.empty())
						{
							DrawVec2("Scale", m_EntitySelected.transformComponent->fakeScale, 0.1f, 1.f);
							m_EntitySelected.transformComponent->scale = m_EntitySelected.transformComponent->fakeScale + m_EntitySelected.spriteComponent->size;
						}
					}
					else
						DrawVec2("Scale", m_EntitySelected.transformComponent->scale, 0.1f, 1.f);
					DrawSliderFloat("Rotation", m_EntitySelected.transformComponent->rotation, 0.f, -360.f, 360.f);
				}
			}

			/*** Camera ***/
			if (m_EntitySelected.cameraComponent)
			{
				ImGui::Separator();

				if (ImGui::CollapsingHeader(ICON_FA_VIDEO"  Camera", headerFlags))
				{
					if (!DeleteComponent<CameraComponent>(m_EntitySelected.entity))
					{
						DrawCheckBox("Current", &m_EntitySelected.cameraComponent->current);
						//float preOrthoSize = camera->Camera.GetOrthoSize();
						//float orthoSize = preOrthoSize;
						if (DrawDragFloat("OrthoSize", m_EntitySelected.cameraComponent->camera.m_OrthoSize, 1.f, 500.f))
							m_EntitySelected.cameraComponent->camera.SetOrthoSize(m_EntitySelected.cameraComponent->camera.m_OrthoSize);
					}
					else m_EntitySelected.cameraComponent = nullptr;
				}
			}

			/*** Sprite ***/
			if (m_EntitySelected.spriteComponent)
			{
				ImGui::Separator();

				if (ImGui::CollapsingHeader(ICON_FA_GHOST"  Sprite", headerFlags))
				{
					if (!DeleteComponent<SpriteComponent>(m_EntitySelected.entity))
					{
						DrawTextureButton(m_EntitySelected.spriteComponent, m_EntitySelected.transformComponent);

						if (DrawCheckBox("FlipH", &m_EntitySelected.spriteComponent->flipH))
							m_EntitySelected.transformComponent->scale.x = m_EntitySelected.spriteComponent->flipH ? m_EntitySelected.transformComponent->scale.x * -1.f : m_EntitySelected.transformComponent->scale.x * -1.f;
						if (DrawCheckBox("FlipV", &m_EntitySelected.spriteComponent->flipV))
							m_EntitySelected.transformComponent->scale.y = m_EntitySelected.spriteComponent->flipV ? m_EntitySelected.transformComponent->scale.y * -1.f : m_EntitySelected.transformComponent->scale.y * -1.f;

						DrawColorPicker4("Self color", glm::value_ptr(m_EntitySelected.spriteComponent->color.rgba));
						int a = 0;
						DrawSliderInt("Layer", m_EntitySelected.spriteComponent->layer, 0, -20, 20);
					}
					else m_EntitySelected.spriteComponent = nullptr;
				}
			}

			if (m_EntitySelected.pyScriptComponent)
			{
				ImGui::Separator();

				if (ImGui::CollapsingHeader(ICON_FA_CODE"  PyScripts", headerFlags))
				{
					if (!DeleteComponent<PyScriptComponent>(m_EntitySelected.entity))
					{
						if (m_EntitySelected.pyScriptComponent->filePath.empty())
						{
							if (ImGui::Button("Create"))
							{
								auto path = std::filesystem::current_path().string() + "//assets//scripts//" + m_EntitySelected.entity->Tag + ".py";
								std::ofstream file;
								file.open(path);
								file << "import PyScripting\n";
								file << "from PyScripting import Math, Input\n\n";
								file << "#VERY IMPORTANT DO NOT CHANGE THE FILE NAME.\n\n";
								file << "class " << m_EntitySelected.entity->Tag << "(PyScripting.PyScript):\n";
								file << "\ttransform = None\n";
								file << "\tdef OnStart(self):\n\t\tself.transform = self.entity.GetTransformComponent()\n";
								file << "\tdef OnUpdate(self, _delta):\n\t\t#update\n\t\tpass\n";
								file << "\tdef OnQuit(self):\n\t\tpass\n";
								file.close();
								m_EntitySelected.pyScriptComponent->filePath = path;
								m_EntitySelected.pyScriptComponent->moduleName = m_EntitySelected.entity->Tag;
								m_EntitySelected.pyScriptComponent->CreateScript(std::make_shared<PyScript>(m_EntitySelected.pyScriptComponent->moduleName, m_EntitySelected.entity));

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
										m_EntitySelected.pyScriptComponent->filePath = castedData->Path.string();
										m_EntitySelected.pyScriptComponent->moduleName = castedData->Path.stem().string();
										m_EntitySelected.pyScriptComponent->CreateScript(std::make_shared<PyScript>(m_EntitySelected.pyScriptComponent->moduleName, m_EntitySelected.entity));
										CodeEditorPanel::PushFile(m_EntitySelected.pyScriptComponent->filePath);
									}
								}
								ImGui::EndDragDropTarget();
							}
						}
						else
						{
							ImGui::Button((m_EntitySelected.entity->Tag + ".py").c_str());
						}
					}
					else m_EntitySelected.pyScriptComponent = nullptr;
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
						m_EntitySelected.spriteComponent = m_EntitySelected.entity->AddComponent<SpriteComponent>();
					}
					if (ImGui::Selectable("Camera Component"))
					{
						m_EntitySelected.cameraComponent = m_EntitySelected.entity->AddComponent<CameraComponent>(m_CurrentScene->m_Size.x / m_CurrentScene->m_Size.y);
					}
					if (ImGui::Selectable("PyScript Component"))
					{
						m_EntitySelected.pyScriptComponent = m_EntitySelected.entity->AddComponent<PyScriptComponent>();
					}
					ImGui::EndPopup();
				}
			}
		}
		ImGui::End();
	}
}
