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
		PolygonShapeComponent* polygonShape = entity->GetComponent<PolygonShapeComponent>();
		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		float circleSize = std::clamp(editorCamera.GetZoomLevel() * 0.05f, 15.f, 400.f);
		if (sprite)
		{
			SpriteComponent overlaySprite = *sprite;
			overlaySprite.color = Color::NICE_BLUE;
			auto overlayTransformCom = transform;
			auto overlayTransform = overlayTransformCom->GetTransform();

			Renderer2D::DrawWiredQuad(overlayTransform, overlaySprite.color, 22);
			static constexpr auto dotOffset = 5.f;
			glm::vec2 top1 = { overlayTransformCom->translation.x - (overlayTransformCom->scale.x * 0.5f), overlayTransformCom->translation.y + (overlayTransformCom->scale.y * 0.5f) }; // top right
			glm::vec2 top2 = { overlayTransformCom->translation.x + (overlayTransformCom->scale.x) * 0.5f, overlayTransformCom->translation.y + (overlayTransformCom->scale.y) * 0.5f }; // top left
			glm::vec2 botton1 = { overlayTransformCom->translation.x - (overlayTransformCom->scale.x) * 0.5f, overlayTransformCom->translation.y - (overlayTransformCom->scale.y) * 0.5f }; // bottom right
			glm::vec2 botton2 = { overlayTransformCom->translation.x + (overlayTransformCom->scale.x) * 0.5f, overlayTransformCom->translation.y - (overlayTransformCom->scale.y) * 0.5f }; // bottom left

			auto top1Transform = glm::translate(glm::mat4(1.0f), glm::vec3(overlayTransformCom->translation, 0.f))
				* glm::rotate(glm::mat4(1.f), glm::radians(overlayTransformCom->rotation), { 0.f, 0.f, 1.f })
				* glm::translate(glm::mat4(1.0f), glm::vec3(top1 - overlayTransformCom->translation, 0.f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(circleSize, circleSize, 0.0f));

			auto top2Transform = glm::translate(glm::mat4(1.0f), glm::vec3(overlayTransformCom->translation, 0.f))
				* glm::rotate(glm::mat4(1.f), glm::radians(overlayTransformCom->rotation), { 0.f, 0.f, 1.f })
				* glm::translate(glm::mat4(1.0f), glm::vec3(top2 - overlayTransformCom->translation, 0.f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(circleSize, circleSize, 0.0f));

			auto botton1Transform = glm::translate(glm::mat4(1.0f), glm::vec3(overlayTransformCom->translation, 0.f))
				* glm::rotate(glm::mat4(1.f), glm::radians(overlayTransformCom->rotation), { 0.f, 0.f, 1.f })
				* glm::translate(glm::mat4(1.0f), glm::vec3(botton1 - overlayTransformCom->translation, 0.f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(circleSize, circleSize, 0.0f));

			auto botton2Transform = glm::translate(glm::mat4(1.0f), glm::vec3(overlayTransformCom->translation, 0.f))
				* glm::rotate(glm::mat4(1.f), glm::radians(overlayTransformCom->rotation), { 0.f, 0.f, 1.f })
				* glm::translate(glm::mat4(1.0f), glm::vec3(botton2 - overlayTransformCom->translation, 0.f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(circleSize, circleSize, 0.0f));
			//RYM_INFO(c);
			//Renderer2D::DrawCircle(c, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(top1Transform, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(top2Transform, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(botton1Transform, Color::BLUE, 22, entity->ID);
			Renderer2D::DrawCircle(botton2Transform, Color::BLUE, 22, entity->ID);

			if (Input::IsButtonJustPressed(MouseCode::ButtonLeft))
			{
				//RYM_INFO(top1Transform[3][0]);
				auto m = Input::GetCursorWorldPosition();
				m_SpriteCir1 = PointCircle(m.x, m.y, top1Transform[3][0], top1Transform[3][1], circleSize / 2.f);
				m_SpriteCir2 = PointCircle(m.x, m.y, top2Transform[3][0], top2Transform[3][1], circleSize / 2.f);
				m_SpriteCir3 = PointCircle(m.x, m.y, botton1Transform[3][0], botton1Transform[3][1], circleSize / 2.f);
				m_SpriteCir4 = PointCircle(m.x, m.y, botton2Transform[3][0], botton2Transform[3][1], circleSize / 2.f);
			}


			if (m_SpriteCir1 || m_SpriteCir2 || m_SpriteCir3 || m_SpriteCir4)
			{
				blockSelectedEntity = true;
				if (Input::IsButtonPressed(MouseCode::ButtonLeft))
				{
						//+= glm::vec2(m_MouseViewportDelta.x, m_MouseViewportDelta.y);
						auto m = Input::GetMouseWorldDelta();
						float mul = 2.0f;
						if (!sprite->texture.empty())
						{
							if (m_SpriteCir1)
								transform->fakeScale += glm::vec2(-m.x, m.y) * mul;
							else if (m_SpriteCir2)
								transform->fakeScale += glm::vec2(m.x, m.y) * mul;
							else if (m_SpriteCir3)
								transform->fakeScale += glm::vec2(-m.x, -m.y) * mul;
							else
								transform->fakeScale += glm::vec2(m.x, -m.y) * mul;
						}
						else
						{
							if (m_SpriteCir1)
								transform->scale += glm::vec2(-m.x, m.y) * mul;
							else if (m_SpriteCir2)
								transform->scale += glm::vec2(m.x, m.y) * mul;
							else if (m_SpriteCir3)
								transform->scale += glm::vec2(-m.x, -m.y) * mul;
							else
								transform->scale += glm::vec2(m.x, -m.y) * mul;
						}
				}
			}
			else
			{
				blockSelectedEntity = false;
			}
			//RYM_INFO(top);
		}

		if (polygonShape)
		{
			size_t i = 0;
			static glm::vec2* lastPointClicked = nullptr;
			for (auto& p : polygonShape->points)
			{
				auto pos = p + transform->translation;
				Renderer2D::DrawCircle(pos, { circleSize , circleSize }, Color::NICE_BLUE, 22, entity->ID);
				if (Input::IsButtonPressed(MouseCode::ButtonLeft))
				{
					auto m = Input::GetCursorWorldPosition();
					if (PointCircle(m.x, m.y, pos.x, pos.y, circleSize / 2.f) && lastPointClicked == nullptr)
					{
						lastPointClicked = &p;
					}
					if (lastPointClicked == &p)
					{
						blockSelectedEntity = true;
						auto m = Input::GetMouseWorldDelta();
						p += m;
					}
				}
				else
				{
					blockSelectedEntity = false;
					lastPointClicked = nullptr;
				}
				i++;
			}
		}
		Renderer2D::End();
		Renderer2D::EndWire();
		Renderer::SetLineWidth(1.0f);
	}
}
