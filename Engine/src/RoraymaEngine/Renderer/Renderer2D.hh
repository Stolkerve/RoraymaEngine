#pragma once
#include <memory>

#include "Camera.hh"
#include "Texture.hh"
#include "Color.hh"
#include "BatchRendering.hh"
#include "../Scene/Components.hh"

namespace rym
{
	using namespace api;

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void Begin(const glm::mat4& projection);
		static void Begin(const EditorCamera& editorCam);
		static void Begin(const Camera& camera, const TransformComponent& transform);
		static void BeginWire(const EditorCamera& editorCam);
		static void BeginWire(const Camera& camera, const TransformComponent& transform);

		static void End();
		static void EndWire();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, int ID = -1);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const Color& color, int layer = 0, int ID = -1);
		static void DrawQuad(const glm::mat4& transform, const Color& color);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const Color& color);

		static void DrawSprite(SpriteComponent* sprite, TransformComponent* transform, int ID = -1);
		//static void DrawSprite(SpriteComponent* sprite, glm::mat4& transform, int ID = -1);

		static void DrawCircle(const glm::mat4& transform, const Color& color, int layer = 0, int ID = -1);
		static void DrawCircle(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer = 0, int ID = -1);
		static void DrawTriangle(const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawRectTriangle(const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawPolygon(std::vector<glm::vec2>& vertices, const glm::vec2& position, const glm::vec2& size, const Color& color, int layer = 0, int ID = -1);
		static void DrawPolygon(std::vector<glm::vec2>& vertices, const glm::mat4& transform, const Color& color, int layer = 0, int ID = -1);

		static void DrawLine(const std::pair<glm::vec2, glm::vec2>& positions, const Color& color, int layer = 0);
		static void DrawWiredPolygon(std::vector<glm::vec2>& vertices, const glm::vec2& position, const glm::vec2& size, const Color& color);
		static void DrawWiredQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer = 0);
		static void DrawWiredQuad(const glm::mat4& transform, const Color& color, int layer = 0);
	};
}
