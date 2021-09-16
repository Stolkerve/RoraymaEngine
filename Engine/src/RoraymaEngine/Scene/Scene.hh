#pragma once

#include <vector>
#include <queue>
#include <string>
#include <unordered_set>

#include "NativeScript.hh"
#include "PyScript.hh"
#include "../Renderer/Camera.hh"
#include "Nodes.hh"
namespace rym
{
	class Scene
	{
	public:
		Scene();
		Scene(const std::string& name);
		~Scene();

		void CreateEntity(const std::string& name);
		void CreateEntity(const Entity* srcEntity, size_t where);
		void CreateEntity(const std::string& name, Entity* retrivedEntity);
		void CreateSprite(const std::string& name);
		void CreateSprite(const std::string& name, Entity* retrivedEntity);
		void CreateCamera(const std::string& name);
		void CreateCamera(const std::string& name, Entity* retrivedEntity);
		void CreatePolygonShape(const std::string& name);
		void CreatePolygonShape(const std::string& name, Entity* retrivedEntity);
		void CreateText(const std::string& name, Entity* retrivedEntity);
		void DeleteEntity(const std::string& name);
		Entity* GetEntity(const std::string& name);
		Entity* GetEntity(uint32_t ID);
		Entity* CreateEmptyEntity(const std::string& name);

		void OnUpdateEditor(const EditorCamera& editorCamera);
		void OnStartGame();
		void OnUpdateGame(float _delta);
		void OnQuitGame();
		void OnViewportResize(uint32_t width, uint32_t height);
		void SetMainCamera(const Entity* entity);

		std::string Name = "Untitle";
	private:
		std::string CheckEntitysName(const std::string& name);

	private:
		CameraComponent* m_MainCamera = nullptr;
		TransformComponent* m_MainCameraTransform = nullptr;

		glm::vec2 m_Size;

		std::vector<Entity*> m_Entitys;
		//std::vector<std::shared_ptr<Entity>> m_ScriptingEntitys;
		std::unordered_set<Entity*> m_ScriptingEntitys;
		std::queue<uint32_t> m_AvailableEntities;
		friend class EntitysPanel;
		friend class SceneSerializer;
		friend class FoldersPanel;
		friend class PropertiesPanel;
	};
}
