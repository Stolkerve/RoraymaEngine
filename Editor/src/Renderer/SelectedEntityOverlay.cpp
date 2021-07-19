#include "SelectedEntityOverlay.hh"
#include <RoraymaEngine/Renderer/Renderer2D.hh>
#include <RoraymaEngine/Scene/AssetsManager.hh>
#include <RoraymaEngine/Core/Input.hh>
#include <imgui/imgui.h>
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
		Renderer2D::BeginWire(editorCamera);
		Renderer2D::Begin(editorCamera);
		SpriteComponent* sprite = entity->GetComponent<SpriteComponent>();
		if (sprite)
		{
			TransformComponent* spriteTransformCmp = entity->GetComponent<TransformComponent>();
			glm::mat4 overlayTransform = (*spriteTransformCmp).GetTransform();
			SpriteComponent overlaySprite = *sprite;
			overlaySprite.color = Color::BLUE;
			Renderer2D::DrawWiredQuad(overlayTransform, overlaySprite.color, 22);

			glm::vec2 top1 = { overlayTransform[3].x - (overlayTransform[0][0] * 0.5f), overlayTransform[3].y + (overlayTransform[1][1] * 0.5f) };
			glm::vec2 top2 = { overlayTransform[3].x + (overlayTransform[0][0]) * 0.5f, overlayTransform[3].y + (overlayTransform[1][1]) * 0.5f };
			glm::vec2 botton1 = { overlayTransform[3].x - (overlayTransform[0][0]) * 0.5f, overlayTransform[3].y - (overlayTransform[1][1]) * 0.5f };
			glm::vec2 botton2 = { overlayTransform[3].x + (overlayTransform[0][0]) * 0.5f, overlayTransform[3].y - (overlayTransform[1][1]) * 0.5f };

			float circleSize = std::clamp(editorCamera.GetZoomLevel() * 0.05f, 15.f, 400.f);
			Renderer2D::DrawCircle(top1, { circleSize,circleSize }, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(top2, { circleSize,circleSize }, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(botton1, { circleSize, circleSize }, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(botton2, { circleSize, circleSize }, Color::BLUE, 22, entity->ID);
			static bool c1 = false;
			static bool c2 = false;
			static bool c3 = false;
			static bool c4 = false;

			if (Input::IsButtonJustPressed(MouseCode::ButtonLeft))
			{
				auto m = Input::GetCursorWorldPosition();
				c1 = PointCircle(m.x, m.y, top1.x, top1.y, circleSize / 2.f);
				c2 = PointCircle(m.x, m.y, top2.x, top2.y, circleSize / 2.f);
				c3 = PointCircle(m.x, m.y, botton1.x, botton1.y, circleSize / 2.f);
				c4 = PointCircle(m.x, m.y, botton2.x, botton2.y, circleSize / 2.f);
			}


			if (c1 || c2 || c3 || c4)
			{
				blockSelectedEntity = true;
				if (Input::IsButtonPressed(MouseCode::ButtonLeft))
				{
						//+= glm::vec2(m_MouseViewportDelta.x, m_MouseViewportDelta.y);
						auto m = Input::GetMouseWorldDelta();
						float mul = 2.0f;
						if (!sprite->texture.empty())
						{
							if (c1)
								spriteTransformCmp->fakeScale += glm::vec2(-m.x, m.y) * mul;
							else if (c2)
								spriteTransformCmp->fakeScale += glm::vec2(m.x, m.y) * mul;
							else if (c3)
								spriteTransformCmp->fakeScale += glm::vec2(-m.x, -m.y) * mul;
							else
								spriteTransformCmp->fakeScale += glm::vec2(m.x, -m.y) * mul;
						}
						else
						{
							if (c1)
								spriteTransformCmp->scale += glm::vec2(-m.x, m.y) * mul;
							else if (c2)
								spriteTransformCmp->scale += glm::vec2(m.x, m.y) * mul;
							else if (c3)
								spriteTransformCmp->scale += glm::vec2(-m.x, -m.y) * mul;
							else
								spriteTransformCmp->scale += glm::vec2(m.x, -m.y) * mul;
						}
				}
			}
			else
			{
				blockSelectedEntity = false;
			}
			//RYM_INFO(top);
		}
		Renderer2D::End();
		Renderer2D::EndWire();
		Renderer::SetLineWidth(1.0f);
	}
}
