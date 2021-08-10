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
		void EntityClicked(const std::shared_ptr<Entity>& entity);
		void Render();
		void SetSceneEntitySelectedCallback(std::function<void(const std::shared_ptr<Entity>&)> fun);
		//std::function<void(const std::shared_ptr<Entity>&)>
	private:
		PropertiesPanel m_PropertiesPanel;
		void AddNode(std::shared_ptr<Entity> entity, size_t id);

		std::shared_ptr<Entity> m_SelectedEntity = nullptr;
		std::shared_ptr<Scene> m_CurrentScene;
		std::function<void(const std::shared_ptr<Entity>&)> m_SetSceneEntitySelected;
	};
}
