#include "Renderer.hh"
#include "../Core/App.hh"
#include "../Core/Log.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace rym
{
	namespace api
	{
		Caps* Renderer::RendererCaps = new Caps;

		void OpenGLMessageCallback(
			unsigned source,
			unsigned type,
			unsigned id,
			unsigned severity,
			int length,
			const char* message,
			const void* userParam)
		{
			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:         RYM_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       RYM_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          RYM_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: RYM_CORE_TRACE(message); return;
			}

			//RYM_CORE_ASSERT(false, "Unknown severity level!");
			//RYM_CORE_ASSERT(false, "Unknown severity level!");
		}

		void Renderer::Init()
		{
			RYM_CORE_INFO("OpenGL version {}", glGetString(GL_VERSION));
			RYM_CORE_INFO("Graphic Card {}", glGetString(GL_VENDOR));
			RYM_CORE_INFO("Currend vender {}", glGetString(GL_RENDERER));

			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(OpenGLMessageCallback, nullptr);

			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

			glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_LESS);

			// Stencil Test
			//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

			// Blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glGetIntegerv(GL_MAX_TEXTURE_SIZE, &RendererCaps->MaxTextureSize);
			//RYM_CORE_INFO(RendererCaps->MaxTextureSize);
			glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &RendererCaps->MaxTextureUnits);
			//RYM_CORE_INFO(RendererCaps->MaxTextureUnits);
		}

		void Renderer::Clear(const glm::vec4& color)
		{
			glClearColor(color.x, color.y, color.z, color.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void Renderer::DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t first, uint32_t count, int mode)
		{
			vertexArray->Bind();
			glDrawArrays(mode, 0, count);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Renderer::DrawIndex(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, int mode)
		{
			vertexArray->Bind();
			glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		void Renderer::DrawIndexWireLine(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count)
		{
			vertexArray->Bind();
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			glViewport(x, y, width, height);
		}

		void Renderer::windowResize(uint32_t width, uint32_t height)
		{
			Renderer::SetViewport(0, 0, width, height);
		}

		void Renderer::EnableWireFrame()
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		void Renderer::SetLineWidth(float width)
		{
			glLineWidth(width);
		}

		void Renderer::DisableWireFrame()
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glm::vec2 Renderer::GetFrameBufferSize()
		{
			glm::ivec2 f = { 0, 0 };
			glfwGetFramebufferSize(rym::App::GetInstance().GetWindow()->GetContextWindow(), &f.x, &f.y);
			return f;
		}

		glm::vec2 Renderer::GetWindowsSize()
		{
			glm::ivec2 f = { 0, 0 };
			glfwGetWindowSize(rym::App::GetInstance().GetWindow()->GetContextWindow(), &f.x, &f.y);
			return f;
		}
	}
}

