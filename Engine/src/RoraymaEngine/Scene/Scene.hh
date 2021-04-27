#pragma once

#include <vector>
#include <queue>
#include <string_view>

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
		Scene(const std::string_view& name);
		~Scene();

		void CreateEntity(const std::string_view& name);
		void CreateEntity(const std::shared_ptr<Entity>& srcEntity, size_t where);
		void CreateEntity(const std::string_view& name, std::shared_ptr<Entity>& retrivedEntity);
		void CreateSprite(const std::string_view& name);
		void CreateSprite(const std::string_view& name, std::shared_ptr<Entity>& retrivedEntity);
		void CreateCamera(const std::string_view& name);
		void CreateCamera(const std::string_view& name, std::shared_ptr<Entity>& retrivedEntity);
		void DeleteEntity(const std::string_view& name);
		std::shared_ptr<Entity> GetEntity(const std::string_view& name);
		std::shared_ptr<Entity> GetEntity(uint32_t ID);

		void OnUpdateEditor(const EditorCamera& editorCamera);
		void OnStartGame();
		void OnUpdateGame(float _delta);
		void OnQuitGame();
		void OnViewportResize(uint32_t width, uint32_t height);

		std::string Name = "Untitle";
	private:
		std::shared_ptr<Entity> CreateEmptyEntity(const std::string_view& name);
		std::string CheckEntitysName(const std::string_view& name);
		CameraComponent* GetFirstCamera();

	private:
		glm::vec2 m_Size;

		std::vector<std::shared_ptr<Entity>> m_Entitys;
		std::vector<std::shared_ptr<Entity>> m_ScriptingEntitys;
		std::queue<uint32_t> m_AvailableEntities;
		friend class EntitysPanel;
		friend class SceneSerializer;
		friend class FoldersPanel;
		friend class PropertiesPanel;
	};
}
