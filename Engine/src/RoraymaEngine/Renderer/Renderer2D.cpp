#include "Renderer2D.hh"

#include "../Scene/AssetsManager.hh"
#include "../Core/Clock.hh"

namespace rym
{
	using namespace api;

	static Polygons* s_PolygonsBatching;
	static WirePolygons* s_WirePolygonsBatching;

	void Renderer2D::Init()
	{
		AssetsManager::PushShader("TextureShader", "assets/shaders/TextureShader.glsl");
		uint32_t whiteTextureData = 0xffffffff;
		AssetsManager::PushTexture("WhiteTexture", 1, 1, &whiteTextureData);
		AssetsManager::PushTexture("CameraEditor", "assets/Textures/camera.png");
		AssetsManager::PushTexture("GhostEditor", "assets/Textures/ghost.png");

		s_PolygonsBatching = new Polygons;
		s_WirePolygonsBatching = new WirePolygons;
	}

	void Renderer2D::Shutdown()
	{
		delete s_PolygonsBatching;
		delete s_WirePolygonsBatching;
	}

	void Renderer2D::Begin(const glm::mat4& projection)
	{
		s_PolygonsBatching->Begin(projection);
	}

	void Renderer2D::Begin(const EditorCamera& editorCam)
	{
		s_PolygonsBatching->Begin(editorCam.GetViewProjectionMatrix());
	}


	void Renderer2D::Begin(const Camera& camera, const TransformComponent& transform)
	{
		//glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);
		auto trasnformMatrix = glm::translate(glm::mat4(1.f), glm::vec3(transform.translation, 0.f)) *
			glm::toMat4(glm::quat(glm::vec3(0.f, 0.f, glm::radians(transform.rotation))));


		//glm::mat4 viewProjection = camera.GetProjection() * trasnformMatrix;
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(trasnformMatrix);

		s_PolygonsBatching->Begin(viewProjection);
	}

	void Renderer2D::BeginWire(const EditorCamera& editorCam)
	{
		s_WirePolygonsBatching->Begin(editorCam.GetViewProjectionMatrix());
	}

	void Renderer2D::BeginWire(const Camera& camera, const TransformComponent& transform)
	{
		//glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(transform);
		auto trasnformMatrix = glm::translate(glm::mat4(1.f), glm::vec3(transform.translation, 0.f)) *
			glm::toMat4(glm::quat(glm::vec3(0.f, 0.f, glm::radians(transform.rotation))));


		//glm::mat4 viewProjection = camera.GetProjection() * trasnformMatrix;
		glm::mat4 viewProjection = camera.GetProjection() * glm::inverse(trasnformMatrix);

		s_WirePolygonsBatching->Begin(viewProjection);
	}

	void Renderer2D::End()
	{
		s_PolygonsBatching->Flush();
	}

	void Renderer2D::EndWire()
	{
		s_WirePolygonsBatching->Flush();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, int ID)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_PolygonsBatching->DrawQuad(transform, color, ID);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const std::shared_ptr<Texture2D>& texture, const Color& color, int layer, int ID)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_PolygonsBatching->DrawQuad(transform, texture, color, layer, ID);
	}

	void Renderer2D::DrawSprite(SpriteComponent* sprite, TransformComponent* transform, int ID)
	{
		auto spriteTransform = transform->GetTransform();
		const auto& texture = AssetsManager::GetTexture(sprite->texture);
		if (texture.get() != nullptr)
		{
			float textureWidth = static_cast<float>(texture->GetWidth());
			float textureHeight = static_cast<float>(texture->GetHeight());
			sprite->size = { textureWidth + transform->scale.x, textureHeight + transform->scale.y };
			spriteTransform *= glm::scale(glm::mat4(1.f), { textureWidth, textureHeight, 0.f });

			s_PolygonsBatching->DrawQuad(spriteTransform, texture, sprite->color, sprite->layer, ID);
		}
		else
		{
			sprite->size = { transform->scale.x, transform->scale.y };
			s_PolygonsBatching->DrawQuad(spriteTransform, sprite->color, sprite->layer, ID);
		}
	}

	//void Renderer2D::DrawSprite(SpriteComponent* sprite, glm::mat4& transform, int ID)
	//{
	//	s_PolygonsBatching->DrawQuad(transform, sprite->color, ID);
	//}

	void Renderer2D::DrawPolygon(std::vector<glm::vec2>& vertices, const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_PolygonsBatching->DrawPolygon(vertices, transform, color);
	}
	void Renderer2D::DrawPolygon(std::string name, std::vector<glm::vec2>& vertices, const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_PolygonsBatching->DrawPolygon(name, vertices, transform, color);
	}

	void Renderer2D::DrawRectTriangle(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_PolygonsBatching->DrawRectTriangle(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Color& color)
	{
		s_PolygonsBatching->DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const Color& color)
	{
		s_PolygonsBatching->DrawQuad(transform, texture, color);
	}

	void Renderer2D::DrawCircle(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_PolygonsBatching->DrawCircle(transform, color);
	}

	void Renderer2D::DrawTriangle(const glm::vec2& position, const glm::vec2& size, const Color& color)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_PolygonsBatching->DrawTriangle(transform, color);
	}

	void Renderer2D::DrawWiredPolygon(std::vector<glm::vec2>& vertices, const glm::vec2& position, const glm::vec2& size, const Color& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_WirePolygonsBatching->Draw(vertices, transform, color);
	}

	void Renderer2D::DrawWiredQuad(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), { position.x, position.y, 0.f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_WirePolygonsBatching->DrawQuad(transform, color, layer);
	}

	void Renderer2D::DrawWiredQuad(const glm::mat4& transform, const Color& color, int layer)
	{
		s_WirePolygonsBatching->DrawQuad(transform, color, layer);
	}

	void Renderer2D::DrawLine(const std::pair<glm::vec2, glm::vec2>& positions, const Color& color, int layer)
	{
		s_WirePolygonsBatching->DrawLine(positions, color, layer);
	}
}
