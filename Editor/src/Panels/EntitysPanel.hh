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
		void EntityClicked(Entity* entity);
		void Render();
		void SetSceneEntitySelectedCallback(std::function<void(Entity*)> fun);
		//std::function<void(const std::shared_ptr<Entity>&)>
		PropertiesPanel m_PropertiesPanel;
	private:
		void AddNode(Entity* entity, size_t id);

		Entity* m_SelectedEntity = nullptr;
		std::shared_ptr<Scene> m_CurrentScene;
		std::function<void(Entity*)> m_SetSceneEntitySelected;
	};
}
