#pragma once

#include<string>
#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Renderer/Texture.hh"
#include "../Renderer/Color.hh"
#include "../Renderer/Camera.hh"
#include "../Renderer/Characters.hh"

namespace rym
{
	using namespace api;

	using ComponentType = std::size_t;

	// Transform component
	struct TransformComponent
	{
		glm::vec2 translation = { 0.0f, 0.0f };
		glm::vec2 scale = { 1.0f, 1.0f };
		glm::vec2 fakeScale = { 1.0f, 1.0f };
		float rotation = 0.0f ;

		TransformComponent() = default;
		TransformComponent(const TransformComponent& copy) :
			translation(copy.translation), scale(copy.scale), rotation(copy.rotation)
		{}
		TransformComponent(const glm::vec2 & translation)
			: translation(translation) {}

		glm::mat4 GetTransform() const
		{
			return glm::translate(glm::mat4(1.0f), glm::vec3(translation, 0.f))
				* glm::toMat4(glm::quat(glm::vec3(0.f, 0.f, glm::radians(rotation))))
				* glm::scale(glm::mat4(1.0f), glm::vec3(scale, 0.0f));
		}
		static ComponentType GetType();
	};

	// Sprite component
	struct SpriteComponent
	{
		std::string texture;
		rym::Color color = rym::Color::WHITE;
		glm::vec2 size = {0.f, 0.f};
		bool flipH = false, flipV = false;
		int layer = 0;

		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent& copy) :
			texture(copy.texture), color(copy.color),
			flipH(copy.flipH), flipV(copy.flipV) {}
		SpriteComponent(const rym::Color& color) :
			color(color) {}
		SpriteComponent(const std::string& texture) :
			texture(texture) {}

		static ComponentType GetType();
	};

	struct CameraComponent
	{
		bool current = false;

		Camera2D camera;
		rym::Color color = rym::Color::WHITE; // implement its later
		CameraComponent() = default;
		CameraComponent(float aspecRatio) :
			camera(aspecRatio) {}

		static ComponentType GetType();
	};

	class NativeScript;
	struct NativeScriptComponent
	{
		NativeScript* instance = nullptr; // its only exits to call the OnUpdate func onces.

		NativeScriptComponent() = default;
		NativeScriptComponent(const NativeScriptComponent&) = default;
		NativeScriptComponent(NativeScript* instace) :
			ptr(instace) {}

		void InstaceScript()
		{
			instance = ptr;
		}
		static ComponentType GetType();

	private:
		NativeScript* ptr = nullptr;
	};

	class PyScript;
	struct PyScriptComponent
	{
		PyScriptComponent() = default;
		PyScriptComponent(std::shared_ptr<PyScript>&& module)
		{
			ptr = std::move(module);
		}

		void CreateScript(std::shared_ptr<PyScript>&& module)
		{
			ptr = std::move(module);
		}

		void DeleteScript()
		{
			if (ptr)
			{
				//delete ptr;
			}
		}

		bool start = false;
		std::shared_ptr<PyScript> ptr;
		std::string filePath;
		std::string moduleName;

		static ComponentType GetType();
	};

	struct PolygonShapeComponent {
		std::vector<glm::vec2> points;
		rym::Color color = rym::Color::WHITE;
		int layer = 0;
		static ComponentType GetType();
	};

	struct TextComponent {
		TextComponent()
			//: characters("assets/theme/fonts/Antonio-Bold.ttf")
		{

		}
		//Characters characters;
		std::string font = "Default";
		glm::vec2 pos = { 0.f, 0.f };
		glm::vec2 scale = { 1.f, 1.f };
		//float fontSize = 20;
		int layer = 0;
		std::string text;
		rym::Color color = rym::Color::WHITE;
		static ComponentType GetType();
	};
}
