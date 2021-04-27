#pragma once

#include "Camera.hh"
#include "VertexArray.hh"
#include "Renderer.hh"
#include "ShaderProgram.hh"

namespace rym
{
	/*
	*/
	class DynamicGrid
	{
	public:
		DynamicGrid();
		void Render(const EditorCamera& camera);
	private:
		std::shared_ptr<api::VertexArray> m_VertexArray;
		std::shared_ptr<api::VertexBuffer> m_VertexBuffer;
		std::shared_ptr<api::ShaderProgram> m_Shader;
	};
}
