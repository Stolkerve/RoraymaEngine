#pragma once

#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace rym
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) :
			m_ProjectionMatrix(projection) {}

		glm::mat4 GetProjection() const { return m_ProjectionMatrix; }
	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.f);
	};

	class Camera2D : public Camera
	{
	public:
		Camera2D() = default;
		Camera2D(float aspectRatio);
		~Camera2D();

		void SetOrthoSize(float orthoSize);
		const float GetOrthoSize() const { return m_OrthoSize; }
		const float GetAspectRatio() const { return m_AspectRatio; }

		void Resize(float aspecRatio);

		struct Rect
		{
			float left, right, bottom, top;
		}m_Rect;
		float m_OrthoSize = 500.f;
	private:
		void UpdateMatrix();

	private:
		float m_AspectRatio;
	};

	class EditorCamera
	{
	public:
		EditorCamera();
		~EditorCamera();

		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void Update(glm::vec2 viewport);
		float GetZoomLevel() const { return m_ZoomLevel; }
		void Zoom(float zoom);
		void Resize(float width, float height);
		const float GetAspectRatio() const { return m_AspectRatio; }
	private:
		void UpdateMatrix();
		void PanMouse(glm::vec2 delta);
		glm::vec2 PanSpeed();

	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position;

		glm::vec2 m_Viewport;

		float m_AspectRatio;
		float m_ZoomLevel;
	};

}
