#include "SelectedEntityOverlay.hh"
#include <RoraymaEngine/Renderer/Renderer2D.hh>
#include <RoraymaEngine/Scene/AssetsManager.hh>
#include <RoraymaEngine/Core/Input.hh>

bool PointCircle(float px, float py, float cx, float cy, float r) {

	// get distance between the point and circle's center
	// using the Pythagorean Theorem
	float distX = px - cx;
	float distY = py - cy;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the circle's
	// radius the point is inside!

	if (distance <= r) {
		return true;
	}
	return false;
}

namespace rym
{
	void SEOverlay::DrawOverlay(const std::shared_ptr<Entity>& entity, const EditorCamera& editorCamera, bool& blockSelectedEntity)
	{
		Renderer::SetLineWidth(1.6f);
		Renderer2D::Begin(editorCamera);
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

			glm::vec2 top1 = { spriteTransform[3].x - (spriteTransform[0][0] * 0.5f), spriteTransform[3].y + (spriteTransform[1][1] * 0.5f) };
			glm::vec2 top2 = { spriteTransform[3].x + (spriteTransform[0][0]) * 0.5f, spriteTransform[3].y + (spriteTransform[1][1]) * 0.5f };
			glm::vec2 botton1 = { spriteTransform[3].x - (spriteTransform[0][0]) * 0.5f, spriteTransform[3].y - (spriteTransform[1][1]) * 0.5f };
			glm::vec2 botton2 = { spriteTransform[3].x + (spriteTransform[0][0]) * 0.5f, spriteTransform[3].y - (spriteTransform[1][1]) * 0.5f };

			Renderer2D::DrawCircle(top1, { 15.f, 15.f }, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(top2, { 15.f, 15.f }, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(botton1, { 15.f, 15.f }, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(botton2, { 15.f, 15.f }, Color::BLUE, 22, entity->ID);
			if (Input::IsButtonPressed(MouseCode::ButtonLeft))
			{
				blockSelectedEntity = false;
				static bool isAnyCircleSelected = false;
				if (Input::IsButtonJustPressed(MouseCode::ButtonLeft))
				{
					auto m = Input::GetCursorWorldPosition();
					bool c1 = PointCircle(m.x, m.y, top1.x, top1.y, 15.f / 2.f);
					bool c2 = PointCircle(m.x, m.y, top2.x, top2.y, 15.f / 2.f);
					bool c3 = PointCircle(m.x, m.y, botton1.x, botton1.y, 15.f / 2.f);
					bool c4 = PointCircle(m.x, m.y, botton2.x, botton2.y, 15.f / 2.f);
					isAnyCircleSelected = c1 || c2 || c3 || c4;
					//RYM_INFO("{0}, {1}, {2}, {3}", c1, c2, c3, c4);
				}

				if (isAnyCircleSelected)
				{
					blockSelectedEntity = true;
					 
				}
			}
			//RYM_INFO(top);
		}
		Renderer2D::End();
		Renderer2D::EndWire();
		Renderer::SetLineWidth(1.0f);
	}
}
