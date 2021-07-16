#include "SelectedEntityOverlay.hh"
#include <RoraymaEngine/Renderer/Renderer2D.hh>
#include <RoraymaEngine/Scene/AssetsManager.hh>
namespace rym
{
	void SEOverlay::DrawOverlay(const std::shared_ptr<Entity>& entity, const EditorCamera& editorCamera)
	{
		Renderer::SetLineWidth(1.6f);
		Renderer2D::BeginWire(editorCamera);
		if (entity->HaveComponent<SpriteComponent>())
		{
			auto spriteTransformCmp = *entity->GetComponent<TransformComponent>();
			auto spriteTransform = spriteTransformCmp.GetTransform();
			SpriteComponent reSprite = *entity->GetComponent<SpriteComponent>();
			const auto& texture = AssetsManager::GetTexture(reSprite.texture);
			reSprite.color = Color::BLUE;
			if (texture.get())
			{
				float spriteWidth = static_cast<float>(texture->GetWidth());
				float spriteHeight = static_cast<float>(texture->GetHeight());
				//spriteTransform.scale = { spriteWidth, spriteHeight };
				spriteTransform *= glm::scale(glm::mat4(1.f), { spriteWidth, spriteHeight, 0.f });
			}
			//Renderer2D::DrawSprite(&reSprite, spriteTransform, entity->ID);
			Renderer2D::DrawWiredQuad(spriteTransform, reSprite.color, 22);
		}
		Renderer2D::EndWire();
		Renderer::SetLineWidth(1.0f);
	}
}
