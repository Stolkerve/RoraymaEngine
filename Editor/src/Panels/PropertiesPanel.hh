#pragma once

#include <RoraymaEngine/Scene/Scene.hh>
#include <RoraymaEngine/Scene/AssetsManager.hh>
#include <imgui/imgui.h>

/*
 Its class exist inside EntitysPanel class as a private member
*/

namespace rym
{
	class PropertiesPanel
	{
	public:
		PropertiesPanel() = default;
		~PropertiesPanel() = default;

		void EntityClicked(std::shared_ptr<Entity> entity)
		{
			m_EntitySelected = entity;
		}

		void Render();

	private:
		std::shared_ptr<Entity> m_EntitySelected = nullptr;
		std::shared_ptr<Scene> m_CurrentScene;

		friend class EntitysPanel;
	};
}
