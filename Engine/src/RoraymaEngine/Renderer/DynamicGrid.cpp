#include "DynamicGrid.hh"
#include "../Scene/AssetsManager.hh"
rym::DynamicGrid::DynamicGrid()
{
	m_VertexArray = std::make_shared<api::VertexArray>();

	std::array buffer = {
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f,-0.5f, 0.0f, 0.0f, 0.0f,
	};

	m_VertexBuffer = std::make_shared<api::VertexBuffer>(buffer.data(), (uint32_t)buffer.size());
	m_VertexBuffer->SetLayout({
		{  api::AttribDataType::Float3, "a_Pos" },
		{  api::AttribDataType::Float2, "a_UV" },
		});
	m_VertexArray->AddVertexBuffer(m_VertexBuffer);

	//Load shader
	AssetsManager::PushShader("DynamicGridShader", "assets/shaders/DynamicGridShader.glsl");
	m_Shader = AssetsManager::GetShader("DynamicGridShader");
}

void rym::DynamicGrid::Render(const EditorCamera& camera)
{
	m_Shader->Bind();
	m_Shader->SetUniformMat4f("u_ViewProjection", camera.GetViewProjectionMatrix());
	m_Shader->SetUniformFloat("u_CameraOrthoSize", camera.GetZoomLevel());

	api::Renderer::DrawArrays(m_VertexArray, 0, 6);

	m_Shader->UnBind();
}
