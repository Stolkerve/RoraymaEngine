#pragma once

#include "VertexArray.hh"
#include "Buffer.hh"

namespace rym
{
	namespace api
	{
		struct Caps
		{
			int MaxTextureUnits;
			int MaxTextureSize;
		};

		class Renderer
		{
		public:
			Renderer(const Renderer&) = delete;
			Renderer& operator=(const Renderer&) = delete;

			static void Init();
			static void Clear(const glm::vec4& color = { 0.f, 0.f , 0.f , 0.f });
			static void DrawArrays(const std::shared_ptr<VertexArray>& vertexArray, uint32_t first, uint32_t count, int mode = 0x0004);
			static void DrawIndex(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count, int mode = 0x0004);
			static void DrawIndexWireLine(const std::shared_ptr<VertexArray>& vertexArray, uint32_t count);
			static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
			static void windowResize(uint32_t width, uint32_t height);
			static void EnableWireFrame();
			static void SetLineWidth(float width);
			static void DisableWireFrame();
			static void PixelStorei(int pname, int value);

			static Caps* RendererCaps;

			static glm::vec2 GetFrameBufferSize();
			static glm::vec2 GetWindowsSize();
		private:
		};
	}
}
