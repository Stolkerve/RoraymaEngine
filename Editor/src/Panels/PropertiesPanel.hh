#pragma once

#include <RoraymaEngine/Scene/Scene.hh>

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

		void EntityClicked(const std::shared_ptr<Entity>& entity)
		{
			m_EntitySelected = entity;
		}

		void Render();

	private:
		//std::shared_ptr<Entity> m_EntitySelected = nullptr;
		std::shared_ptr<Scene> m_CurrentScene;
		//TransformComponent* m_EntityTransform;

		struct EntitySelected{
			std::shared_ptr<Entity> entity;
			TransformComponent* transformComponent;
			CameraComponent* cameraComponent;
			SpriteComponent* spriteComponent;
			PyScriptComponent* pyScriptComponent;
			PolygonShapeComponent* polygonShapeComponent;

			EntitySelected& operator=(const std::shared_ptr<Entity>& other) {
				this->entity = other;
				if (this->entity)
				{
					this->transformComponent = other->GetComponent<TransformComponent>();
					this->cameraComponent = other->GetComponent<CameraComponent>();
					this->spriteComponent = other->GetComponent<SpriteComponent>();
					this->pyScriptComponent = other->GetComponent<PyScriptComponent>();
					this->polygonShapeComponent = other->GetComponent<PolygonShapeComponent>();
				}
				return *this;
			}

		} m_EntitySelected;

		friend class EntitysPanel;
	};
}
