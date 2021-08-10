#include "Camera.hh"
#include "../Core/Assert.hh"
#include "../Core/Input.hh"
#include "../Core/App.hh"
#include <GLFW/glfw3.h>
namespace rym
{
	Camera2D::Camera2D(float aspectRatio) :
		m_AspectRatio(aspectRatio)
	{
		UpdateMatrix();
	}

	void Camera2D::UpdateMatrix()
	{
		m_Rect = {
			-m_OrthoSize * m_AspectRatio, m_OrthoSize * m_AspectRatio, -m_OrthoSize, m_OrthoSize
		};
		//float left = -m_OrthoSize * m_AspectRatio;
		//float right = m_OrthoSize * m_AspectRatio;
		//float bottom = -m_OrthoSize;
		//float top = m_OrthoSize;

		m_ProjectionMatrix = glm::ortho(m_Rect.left, m_Rect.right, m_Rect.bottom, m_Rect.top, -99.f, 99.f);
	}

	/*
	void Camera2D::ClampPosition()
	{
		switch (m_Mode)
		{
		case rym::Mode::TOPLEFT:
			//m_Position.x = glm::clamp(m_Position.x, m_Rect.left, m_Rect.right - float(m_Size.width));
			//m_Position.y = glm::clamp(m_Position.y, m_Rect.top, m_Rect.bottom - float(m_Size.height));
			break;
		case rym::Mode::CENTERED:
			m_Position.x = glm::clamp(m_Position.x, m_Rect.left + m_Size.width * 0.5f, m_Rect.right - m_Size.width * 0.5f);
			m_Position.y = glm::clamp(m_Position.y, m_Rect.top + m_Size.height * 0.5f, m_Rect.bottom - m_Size.height * 0.5f);
			break;
		}
	}
	*/

	void Camera2D::SetOrthoSize(float orthoSize)
	{
		m_OrthoSize = orthoSize;
		UpdateMatrix();
	}

	void Camera2D::Resize(float aspecRatio)
	{
		m_AspectRatio = aspecRatio;
		UpdateMatrix();
	}

	Camera2D::~Camera2D()
	{
	}

	/**** Editor camera ****/

	EditorCamera::EditorCamera() :
		m_ViewMatrix(1.f),
		m_ProjectionMatrix(1.f),
		m_ViewProjectionMatrix(1.f),
		m_Position(0.f),
		m_ZoomLevel(500.f),
		m_AspectRatio(0.f)
	{
		UpdateMatrix();
	}

	EditorCamera::~EditorCamera()
	{
	}

	void EditorCamera::Update(glm::vec2 viewport, glm::vec2 mouseViewport)
	{
		m_Viewport = viewport;

		m_FirstMouseViewport = glm::unProject(
			glm::vec3(mouseViewport.x, viewport.y - mouseViewport.y, 1.f),
			GetViewMatrix(),
			GetProjectionMatrix(),
			glm::vec4(0.f, 0.f, viewport.x, viewport.y));
		m_MouseViewportDelta = m_FirstMouseViewport - m_LastMouseViewport;
		m_LastMouseViewport = m_FirstMouseViewport;

		auto mouse = Input::GetCursorPosition();
		if (Input::IsButtonPressed(MouseCode::ButtonRight))
		{
			//RYM_INFO(mouseViewport);
			PanMouse();
			if (mouseViewport.x <= 0)
			{
				//Input::SetMousePosition({ viewport.x - 10, mouse.y });
				//RYM_INFO("X0");
			}
			else if (mouseViewport.x >= viewport.x - 5)
			{

				//RYM_INFO("X1");
			}
		}

		UpdateMatrix();
	}

	static glm::vec2 GlmNormalizeButWithOutThatFuckingNaNReturn(const glm::vec2& v)
	{
		if (v == glm::vec2(0.f))
			return glm::vec2(0.f);
		else
			return glm::normalize(v);
	}

	void EditorCamera::Zoom(float zoom)
	{
		/*
		m_ZoomLevel += zoom * (m_ZoomLevel * 0.1f);
		m_ZoomLevel = std::max(m_ZoomLevel, 175.1111f);
		//RYM_INFO(m_ZoomLevel);
		*/
		constexpr float MAX_ZOOM = 325.1111f;

		//auto speed = PanSpeed();
		//auto mouse = Input::GetCursorPosition();
		//auto whatever = glm::unProject(
		//	glm::vec3(mouse.x, m_Viewport.y - mouse.y, 1.f),
		//	GetViewMatrix(),
		//	GetProjectionMatrix(),
		//	glm::vec4(0.f, 0.f, m_Viewport.x, m_Viewport.y));
		//whatever -= (whatever - 0.6f);
		//auto whatever2 = GlmNormalizeButWithOutThatFuckingNaNReturn(whatever);

		//if(m_ZoomLevel != MAX_ZOOM && zoom < 0)
			//m_Position += glm::vec3(whatever.x, whatever.y, 0.f);
		m_ZoomLevel += zoom * (m_ZoomLevel * 0.1111f);
		m_ZoomLevel = std::max(m_ZoomLevel, MAX_ZOOM);
	}

	void EditorCamera::SetOrtho(float size)
	{
		m_ZoomLevel = size;
		UpdateMatrix();
	}

	void EditorCamera::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		UpdateMatrix();
	}

	void EditorCamera::UpdateMatrix()
	{
		float left = -m_ZoomLevel * m_AspectRatio;
		float right = m_ZoomLevel * m_AspectRatio;
		float bottom = -m_ZoomLevel;
		float top = m_ZoomLevel;

		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -99.f, 99.f);

		glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.f), m_Position);
		m_ViewMatrix = glm::inverse(transformationMatrix);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void EditorCamera::PanMouse()
	{
		auto delta = m_MouseViewportDelta * 0.5f;
		//RYM_INFO(delta);
		glm::vec2 speed = { 1.f , 1.f };
		m_Position += glm::vec3(delta.x * speed.x, delta.y * speed.y, 0.f) * -1.f;
	}

	/*
	void EditorCamera::SetZoom(const glm::vec2& delta)
	{
		auto speed = PanSpeed();
		auto windowSize = glm::ivec2(0);
		glfwGetWindowSize(App::GetInstance().GetWindow()->GetContextWindow(), &windowSize.x, &windowSize.y);

		auto whatever = glm::vec2((windowSize / 2)) - Input::GetCursorPosition();
		auto whatever2 = GlmNormalizeButWithOutThatFuckingNaNReturns(whatever) * 0.0003f;


		auto scroll = Input::GetMouseScrolled().y;
		//if (scroll)
			//m_Position += glm::vec3(whatever2.x * -1.f * speed.x, whatever2.y * speed.y, 0.f);
		m_Zoom += scroll * (m_Zoom * 0.1f);
		m_Zoom = std::max(m_Zoom, 0.1111f);
	}
	*/
}

