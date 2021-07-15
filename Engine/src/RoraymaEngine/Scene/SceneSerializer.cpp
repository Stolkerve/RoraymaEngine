#include "SceneSerializer.hh"

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <vector>

#include "AssetsManager.hh"
#include "Components.hh"
#include "Entity.hh"

namespace rym
{
	/*
* Scene: "Untitle"
* Entitys:
*	-Entity: "Sprite"
*	 SpriteComponent:
*		FlipH:
*	-Entity: "Sprite"
*	 SpriteComponent:
*		FlipH:
*
*/
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}

	static void SerializeEntity(YAML::Emitter& out, std::shared_ptr<Entity>& entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << entity->Tag;
		/*
			out << YAML::Key << "Position" << YAML::Value << transform->Translation;
			out << YAML::Key << "Position" << YAML::Value << transform->Scale;
			out << YAML::Key << "Position" << YAML::Value << transform->Rotation;
		*/

		if (entity->HaveComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			auto transform = entity->GetComponent<TransformComponent>();
			out << YAML::Key << "Position" << YAML::Value << transform->translation;
			out << YAML::Key << "Scale" << YAML::Value << YAML::Value << transform->scale;
			out << YAML::Key << "Rotation" << YAML::Value << transform->rotation;
			out << YAML::EndMap;
		}

		if (entity->HaveComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;
			auto sprite = entity->GetComponent<SpriteComponent>();
			const auto& texture = AssetsManager::GetTexture(sprite->texture);
			if (texture.get())
			{
				out << YAML::Key << "TextureName" << YAML::Value << sprite->texture;
				out << YAML::Key << "TexturePath" << YAML::Value << texture->GetPath();
			}
			else
			{
				out << YAML::Key << "TextureName" << YAML::Value << "null";
				out << YAML::Key << "TexturePath" << YAML::Value << "null";
			}
			out << YAML::Key << "Color" << YAML::Value << YAML::Value << sprite->color.rgba;
			out << YAML::Key << "FlipH" << YAML::Value << sprite->flipH;
			out << YAML::Key << "FlipV" << YAML::Value << sprite->flipV;
			out << YAML::EndMap;
		}

		if (entity->HaveComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;
			auto camera = entity->GetComponent<CameraComponent>();

			out << YAML::Key << "Current" << YAML::Value << camera->current;
			out << YAML::Key << "Camera2D";
			out << YAML::BeginMap;
			out << YAML::Key << "AspectRatio" << YAML::Value << camera->camera.GetAspectRatio();
			out << YAML::Key << "OrthoSize" << YAML::Value << camera->camera.GetOrthoSize();
			out << YAML::EndMap;

			out << YAML::EndMap;
		}

		if (entity->HaveComponent<PyScriptComponent>())
		{
			auto script = entity->GetComponent<PyScriptComponent>();
			out << YAML::Key << "PyScriptComponent";
			out << YAML::BeginMap;
			out << YAML::Key << "FilePath" << YAML::Value << script->filePath;
			out << YAML::Key << "ModuleName" << YAML::Value << script->moduleName;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::SceneSerialize(const std::shared_ptr<Scene>& scene, const std::string& path)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << scene->Name;
		out << YAML::Key << "Entitys" << YAML::Value << YAML::BeginSeq;

		for (auto e : scene->m_Entitys)
		{
			if (e)
				SerializeEntity(out, e);
		}

		YAML::EndSeq;
		YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
	}

	//TODO: Revisa las copias
	bool SceneSerializer::SceneDesSerialize(const std::shared_ptr<Scene>& scene, const std::string& path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		if (!data["Scene"])
			return false;

		const auto sceneName = data["Scene"].as<std::string>();

		const auto entitys = data["Entitys"];
		if (entitys)
		{
			for (const auto& e : entitys)
			{
				const auto tag = e["Entity"].as<std::string>();
				std::shared_ptr<Entity> newEntity = scene->CreateEmptyEntity(tag);
				scene->m_Entitys.push_back(newEntity);

				const auto serializeTransform = e["TransformComponent"];
				if (serializeTransform)
				{
					newEntity->AddComponent<TransformComponent>();
					TransformComponent* entityTransformComponent = newEntity->GetComponent<TransformComponent>();

					const auto position = serializeTransform["Position"];
					const auto scale = serializeTransform["Scale"];
					const auto rotation = serializeTransform["Rotation"].as<float>();

					entityTransformComponent->translation = { position[0].as<float>(), position[1].as<float>()};
					entityTransformComponent->scale = { scale[0].as<float>(), scale[1].as<float>() };
					entityTransformComponent->rotation = rotation;
				}

				const auto serializeSprite = e["SpriteComponent"];
				if (serializeSprite)
				{
					newEntity->AddComponent<SpriteComponent>();
					SpriteComponent* entitySpriteComponent = newEntity->GetComponent<SpriteComponent>();

					const auto textureName = serializeSprite["TextureName"].as<std::string>();
					const auto texturePath = serializeSprite["TexturePath"].as<std::string>();
					const auto color = serializeSprite["Color"];
					const auto flipH = serializeSprite["FlipH"].as<bool>();
					const auto flipV = serializeSprite["FlipV"].as<bool>();
					if (textureName != "null")
					{
						entitySpriteComponent->texture = textureName;
						AssetsManager::PushTexture(textureName, texturePath);
					}
					entitySpriteComponent->color = { color[0].as<float>(), color[1].as<float>(), color[2].as<float>(), color[3].as<float>() };
					entitySpriteComponent->flipH = flipH;
					entitySpriteComponent->flipV = flipV;
				}

				const auto serializeCamera = e["CameraComponent"];
				if (serializeCamera)
				{
					newEntity->AddComponent<CameraComponent>();
					CameraComponent* entityCameraComponent = newEntity->GetComponent<CameraComponent>();

					const auto current = serializeCamera["Current"].as<bool>();
					const auto camera2D = serializeCamera["Camera2D"];
					const auto orthoSize = camera2D["m_OrthoSize"].as<float>();
					const auto aspectRatio = camera2D["m_OrthoSize"].as<float>();

					entityCameraComponent->current = current;
					entityCameraComponent->camera.SetOrthoSize(orthoSize);
					entityCameraComponent->camera.Resize(aspectRatio);
				}

				const auto serializePyScript = e["PyScriptComponent"];
				if (serializePyScript)
				{
					newEntity->AddComponent<PyScriptComponent>();
					PyScriptComponent* entityPyScriptComponent = newEntity->GetComponent<PyScriptComponent>();
					entityPyScriptComponent->filePath = serializePyScript["FilePath"].as<std::string>();
					entityPyScriptComponent->moduleName = serializePyScript["ModuleName"].as<std::string>();
					entityPyScriptComponent->CreateScript(std::make_shared<PyScript>(entityPyScriptComponent->moduleName, newEntity));
				}
			}
		}
	}
}
