#pragma once

#include "PropertiesPanel.hh"
#include "ConsolePanel.hh"

namespace rym
{
	class EntitysPanel
	{
	public:
		EntitysPanel() = default;
		EntitysPanel(std::shared_ptr<Scene> currentScene);
		~EntitysPanel() = default;

		inline void SetContext(const std::shared_ptr<Scene>& newScene) { m_CurrentScene = newScene; m_PropertiesPanel.m_CurrentScene = newScene; }
		void EntityClicked(std::shared_ptr<Entity> entity);
		void Render();

		
	private:
		PropertiesPanel m_PropertiesPanel;
		void AddNode(std::shared_ptr<Entity> entity, size_t id);

		std::shared_ptr<Entity> SelectedEntity = nullptr;
		std::shared_ptr<Scene> m_CurrentScene;

		ImGuiTreeNodeFlags m_Flags;
	};
}
