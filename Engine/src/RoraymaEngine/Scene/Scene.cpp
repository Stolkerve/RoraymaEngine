#include "Scene.hh"

#include "../Renderer/Renderer2D.hh"
#include "../Scene/AssetsManager.hh"
#include <sstream>

namespace rym
{
	constexpr uint32_t MAX_ENTITYS = 10000;

	Camera2D s_StaticCamera;
	Scene::Scene(const std::string_view& name) :
		Name(name)
	{
		s_StaticCamera.SetOrthoSize(500.f);
		for (uint32_t i = 0; i < MAX_ENTITYS; ++i)
		{
			m_AvailableEntities.push(i);
		}
	}

	Scene::Scene() :
		Name("Unknow_Name")
	{
		s_StaticCamera.SetOrthoSize(500.f);
		for (uint32_t i = 0; i < MAX_ENTITYS; ++i)
		{
			m_AvailableEntities.push(i);
		}
	}

	Scene::~Scene()
	{
		RYM_INFO("Scene destructor is called");
	}

	std::string Scene::CheckEntitysName(const std::string_view& name)
	{
/*
		auto recurSearche = [&]() {
			auto it = std::search()
		};

		auto it = std::find_if(m_Entitys.begin(), m_Entitys.end(), [&name](Entity* e) {
			return e->Tag == name;
			});

		if (it != m_Entitys.end())
			auto a = *it;

		return std::string(name);
*/
		//auto NumOfCopias = [](uint32_t value) {
		//    std::stringstream ss;
		//    ss << "_" << value;
		//    return ss.str();
		//};

		//uint32_t counter = 0;

		//for (auto& e : m_Entitys)
		//{
		//    if (e->Tag == name.data() || (std::string(name) + NumOfCopias(counter)) == e->Tag)
		//        counter++;
		//}

		//if (counter)
		//    return std::string(name) + NumOfCopias(counter);
		return std::string(name);
	}

	std::shared_ptr<Entity> Scene::CreateEmptyEntity(const std::string_view& name)
	{
		auto ptr = std::make_shared<Entity>();
		ptr->Tag = CheckEntitysName(name);

		ptr->ID = m_AvailableEntities.front();
		m_AvailableEntities.pop();

		return ptr;
	}

	void Scene::CreateEntity(const std::string_view& name, std::shared_ptr<Entity>& retrivedEntity)
	{
		auto ptr = CreateEmptyEntity(name);
		retrivedEntity = ptr;

		std::string line = "Entity ";
		line += ptr->Tag;
		line += " Created";

		ptr->AddComponent<TransformComponent>();

		m_Entitys.push_back(ptr);
	}

	void Scene::CreateEntity(const std::string_view& name)
	{
		auto ptr = CreateEmptyEntity(name);

		std::string line = "Entity ";
		line += ptr->Tag;
		line += " Created";

		ptr->AddComponent<TransformComponent>();

		m_Entitys.push_back(ptr);
	}

	void Scene::CreateEntity(const std::shared_ptr<Entity>& srcEntity, size_t where)
	{
		auto name = CheckEntitysName(srcEntity->Tag);
		auto ptr = CreateEmptyEntity(name);

		auto srcTransformComp = srcEntity->GetComponent<TransformComponent>();
		auto srcSpriteComp = srcEntity->GetComponent<SpriteComponent>();
		auto srcCamera2DComp = srcEntity->GetComponent<CameraComponent>();
		auto srcPyScriptComp = srcEntity->GetComponent<PyScriptComponent>();

		ptr->AddComponent<TransformComponent>(*srcTransformComp);
		if (srcSpriteComp)
		{
			ptr->AddComponent<SpriteComponent>(*srcSpriteComp);
		}
		if (srcCamera2DComp)
		{
			ptr->AddComponent<CameraComponent>(srcCamera2DComp->camera.GetAspectRatio());
		}
		if (srcPyScriptComp)
		{
			ptr->AddComponent<PyScriptComponent>(std::make_shared<PyScript>(srcPyScriptComp->moduleName, ptr));
		}

		m_Entitys.insert(m_Entitys.begin() + where, ptr);
	}

	void Scene::CreateSprite(const std::string_view& name, std::shared_ptr<Entity>& retrivedEntity)
	{
		auto ptr = CreateEmptyEntity(name);
		retrivedEntity = ptr;

		std::string line = "Sprite ";
		line += ptr->Tag;
		line += " Created";

		ptr->AddComponent<TransformComponent>();
		ptr->AddComponent<SpriteComponent>(Color::WHITE);

		m_Entitys.push_back(ptr);
	}

	void Scene::CreateSprite(const std::string_view& name)
	{
		auto ptr = CreateEmptyEntity(name);

		std::string line = "Sprite ";
		line += ptr->Tag;
		line += " Created";

		ptr->AddComponent<TransformComponent>();
		ptr->AddComponent<SpriteComponent>(Color::WHITE);

		m_Entitys.push_back(ptr);
	}

	void Scene::CreateCamera(const std::string_view& name, std::shared_ptr<Entity>& retrivedEntity)
	{
		auto ptr = CreateEmptyEntity(name);
		retrivedEntity = ptr;

		std::string line = "Camera ";
		line += ptr->Tag;
		line += " Created";

		ptr->AddComponent<TransformComponent>();
		ptr->AddComponent<CameraComponent>(m_Size.x / m_Size.y);

		m_Entitys.push_back(ptr);
	}

	void Scene::CreateCamera(const std::string_view& name)
	{
		auto ptr = CreateEmptyEntity(name);

		std::string line = "Camera ";
		line += ptr->Tag;
		line += " Created";

		ptr->AddComponent<TransformComponent>();
		ptr->AddComponent<CameraComponent>(m_Size.x / m_Size.y);

		m_Entitys.push_back(ptr);
	}

	void Scene::DeleteEntity(const std::string_view& name)
	{
		std::string line = "Entity ";
		line += name;
		line += " deleted";

		size_t i = 0;
		for (auto& e : m_Entitys)
		{
			if (e->Tag == name)
			{
				// Recover the ID back to the queue
				m_AvailableEntities.push(e->ID);

				m_Entitys.erase(m_Entitys.begin() + i);
				return;
			}
			i++;
		}
	}

	std::shared_ptr<Entity> Scene::GetEntity(const std::string_view& name)
	{
		for (auto& e : m_Entitys)
			if (e->Tag == name)
				return e;
		RYM_CORE_ASSERT(false, "The scene cannot get entity " + std::string(name));
		return nullptr;
	}

	std::shared_ptr<Entity> Scene::GetEntity(uint32_t ID)
	{
		for (auto& e : m_Entitys)
			if (e->ID == ID)
				return e;
		RYM_CORE_ASSERT(false, "The scene cannot get entity ");
		return nullptr;
	}

	void Scene::OnUpdateEditor(const EditorCamera& editorCamera)
	{
		Renderer2D::Begin(editorCamera);
		Renderer2D::BeginWire(editorCamera);
		float camSize = std::clamp(editorCamera.GetZoomLevel() * 0.1f, 50.f, 500.f);
		for (auto& e : m_Entitys)
		{
			if (e->NumOfComponents() == 1)
			{
				auto transform = e->GetComponent<TransformComponent>();
				Renderer2D::DrawQuad(transform->translation, { camSize, camSize }, AssetsManager::GetTexture("GhostEditor"), Color::WHITE, 21, e->ID);
			}

			auto [transform, script, sprite, camera, polygonShape] = std::make_tuple(
				e->GetComponent<TransformComponent>(),
				e->GetComponent<PyScriptComponent>(),
				e->GetComponent<SpriteComponent>(),
				e->GetComponent<CameraComponent>(),
				e->GetComponent<PolygonShapeComponent>()
			);

			if (script)
			{
				if (script->ptr && script->start)
				{
					script->start = false;
				}
			}

			if (sprite)
			{
				// Only in the editor, when a sprite have a texture, the y axis invert because yes.
				Renderer2D::DrawSprite(sprite, transform, e->ID);
			}

			if (camera)
			{
				Renderer2D::DrawQuad(transform->translation, { camSize, camSize }, AssetsManager::GetTexture("CameraEditor"), Color::WHITE, 21, e->ID);
				float right = (camera->camera.GetOrthoSize() * camera->camera.GetAspectRatio()) * 2.f;
				float top = camera->camera.GetOrthoSize() * 2.f;
				Renderer2D::DrawWiredQuad(transform->translation, { right , top }, Color::NICE_BLUE, 21);
			}

			if (polygonShape)
			{
				if(polygonShape->points.size() > 0)
					Renderer2D::DrawPolygon(polygonShape->points, transform->GetTransform(), polygonShape->color, polygonShape->layer, e->ID);
			}
		}

		Renderer2D::EndWire();
		Renderer2D::End();
	}

	void Scene::OnStartGame()
	{
		for (auto& e : m_Entitys)
		{
			if (e->HaveComponent<PyScriptComponent>())
			{
				//m_ScriptingEntitys.insert(e);
			}
		}
	}

	void Scene::OnUpdateGame(float _delta)
	{
		Camera* mainCamera = nullptr;
		TransformComponent* mainCameraTransform = nullptr;
		// Looking for the current cameras (Camera2D)
		{
			for (auto& e : m_Entitys)
			{
				if (e->HaveComponent<CameraComponent>())
				{
					auto [transform, camera] = std::make_tuple(e->GetComponent<TransformComponent>(), e->GetComponent<CameraComponent>());
					//static CameraComponent* previusCamera = nullptr;
					// Get the first current camera
					if (e->visible && camera->current)
					{
						mainCamera = &camera->camera;
						mainCameraTransform = transform;
						break;
					}
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::Begin(*mainCamera, *mainCameraTransform);
		}

		for (auto& e : m_Entitys)
		{
			auto script = e->GetComponent<PyScriptComponent>();
			if (script)
			{
				if (script->ptr)
				{
					if (!script->start)
					{
						script->ptr->m_Entity = e;
						script->ptr->Reload();
						script->ptr->OnStart();
						script->start = true;
					}

					script->ptr->OnUpdate(_delta);
				}
			}

			if (mainCamera)
			{
				auto [transform, sprite, polygonShape] = std::make_tuple(e->GetComponent<TransformComponent>(), e->GetComponent<SpriteComponent>(), e->GetComponent<PolygonShapeComponent>());
				if (e->visible && sprite)
				{
					Renderer2D::DrawSprite(sprite, transform);
				}

				if (e->visible && polygonShape)
				{
					if (polygonShape->points.size() > 0)
						Renderer2D::DrawPolygon(polygonShape->points, transform->GetTransform(), polygonShape->color, polygonShape->layer, e->ID);
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::End();
		}
	}

	void Scene::OnQuitGame()
	{
		/* Its a temporal solution, later I will create a backupsystem when the scene is on game mode*/
		//RYM_INFO("Salio");
		for (size_t i = 0; i < m_Entitys.size(); i++)
		{
			/*
			for (auto e : m_ScriptingEntitys) {
				if (m_Entitys[i]->ID == e->ID)
					m_Entitys[i] = std::move(e);
			}
			*/
		}
		//m_Entitys = std::move(m_EntitysBackup);
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_Size = glm::fvec2{ width, height };

		for (auto& e : m_Entitys)
		{
			if (e->HaveComponent<CameraComponent>())
			{
				auto camera = e->GetComponent<CameraComponent>();
				camera->camera.Resize(m_Size.x / m_Size.y);
			}
		}
	}
}
