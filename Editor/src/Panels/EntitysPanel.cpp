#include "EntitysPanel.hh"
#include <RoraymaEngine/Core/Input.hh>
#include <RoraymaEngine/Core/ImGui/IconsFontAwesome5.h>
#include <imgui/imgui.h>
namespace rym
{
	EntitysPanel::EntitysPanel(std::shared_ptr<Scene> currentScene) :
		m_CurrentScene(currentScene)
	{}

	void EntitysPanel::AddNode(std::shared_ptr<Entity> entity, size_t id)
	{
		const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
		if (ImGui::Button((entity->visible ? ICON_FA_EYE : ICON_FA_EYE_SLASH)))
			entity->visible = !entity->visible;
		ImGui::SameLine(0.f, spacing);
		//(m_SelectedEntity ? ImGuiTreeNodeFlags_Selected : 0) |
		ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_FramePadding;
		bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(id), flags, entity->Tag.c_str());
		static std::shared_ptr<Entity> hoveredEntity = nullptr;
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
			m_PropertiesPanel.EntityClicked(m_SelectedEntity);
		}

		if (ImGui::IsItemHovered())
		{
			hoveredEntity = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
		
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(ICON_FA_TRASH"  Delete entity"))
			{
				m_CurrentScene->DeleteEntity(hoveredEntity->Tag);
				m_PropertiesPanel.EntityClicked(nullptr);
			}
			ImGui::EndPopup();
		}
	}

	void EntitysPanel::EntityClicked(const std::shared_ptr<Entity>& entity)
	{
		m_SelectedEntity = entity;
		m_PropertiesPanel.EntityClicked(m_SelectedEntity);
	}

	void EntitysPanel::Render()
	{
		ImGui::Begin(ICON_FA_SITEMAP"  Entitys panel");

		size_t i = 0;

		if (m_CurrentScene.get())
		{
			//auto& entitysArray = m_CurrentScene->m_Entitys;
			for (size_t i = 0; i < m_CurrentScene->m_Entitys.size(); i++)
			{
				if (m_SelectedEntity &&
					Input::IsKeyPressed(KeysCode::LeftControl) &&
					Input::IsKeyJustPressed(KeysCode::D) &&
					ImGui::IsWindowFocused())
				{
					//m_CurrentScene->m_Entitys.insert(m_CurrentScene->m_Entitys.begin() + i + 1, m_SelectedEntity);
					m_CurrentScene->CreateEntity(m_SelectedEntity, i + 1);
				}

				if (m_CurrentScene->m_Entitys[i])
				{
					ImGui::PushID(static_cast<int>(i));
					AddNode(m_CurrentScene->m_Entitys[i], (size_t)m_CurrentScene->m_Entitys[i].get());
					/*
					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("ENTITYS_PANEL", &i, sizeof(size_t));
						ImGui::Text("This is a drag and drop entity");
						ImGui::EndDragDropSource();
					}
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITYS_PANEL"))
						{
							IM_ASSERT(payload->DataSize == sizeof(size_t));
							int payload_n = *(const int*)payload->Data;

							// it can be moved
							auto tmp = m_CurrentScene->m_Entitys[i];
							m_CurrentScene->m_Entitys[i] = m_CurrentScene->m_Entitys[payload_n];
							m_CurrentScene->m_Entitys[payload_n] = tmp;
						}
						ImGui::EndDragDropTarget();
					}
					*/
					ImGui::PopID();
				}
			}
		}

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Add Empty Entity"))
			{
				m_CurrentScene->CreateEntity("Empty_Entity", m_SelectedEntity);
				m_PropertiesPanel.EntityClicked(m_SelectedEntity);
				m_SetSceneEntitySelected(m_SelectedEntity);
			}
			if (ImGui::MenuItem("Add Sprite"))
			{
				m_CurrentScene->CreateSprite("Sprite", m_SelectedEntity);
				m_PropertiesPanel.EntityClicked(m_SelectedEntity);
				m_SetSceneEntitySelected(m_SelectedEntity);
			}
			if (ImGui::MenuItem("Add Camera"))
			{
				m_CurrentScene->CreateCamera("Camera", m_SelectedEntity);
				m_PropertiesPanel.EntityClicked(m_SelectedEntity);
				m_SetSceneEntitySelected(m_SelectedEntity);
			}
			if (ImGui::MenuItem("Import Scene"))
			{
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		m_PropertiesPanel.Render();
	}

	void EntitysPanel::SetSceneEntitySelectedCallback(std::function<void(const std::shared_ptr<Entity>&)> fun)
	{
		m_SetSceneEntitySelected = fun;
	}
}
