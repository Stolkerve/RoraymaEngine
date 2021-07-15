#pragma once
#include <numeric>

#include <glm/glm.hpp>

#include "Renderer.hh"
#include "Camera.hh"
#include "Color.hh"
#include "ShaderProgram.hh"
#include "Texture.hh"

namespace rym
{
	namespace api
	{
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec4 Color;
			glm::vec2 TexCoord;
			float TexIndex;
			float Layer;
			// Editor only
			int EntityID;
		};

		class BatchStatistics
		{
		public:
			static BatchStatistics& Get();

			int DrawCalls;
			int NumOfVertices;

			void reset();
		};

		class Polygons
		{
		public:
			Polygons();
			~Polygons();

			void DrawQuad(const glm::mat4& transform, const Color& color, int layer = 0, int ID = -1);
			void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const Color& color, int layer = 0, int ID = -1);
			void DrawCircle(const glm::mat4& transform, const Color& color, int ID = -1);
			void DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::mat4& transform, const Color& color, int ID = -1);
			void DrawPolygon(std::string name, const std::vector<glm::vec2>& vertices, const glm::mat4& transform, const Color& color, int ID = -1);
			void DrawTriangle(const glm::mat4& transform, const Color& color, int ID = -1);
			void DrawRectTriangle(const glm::mat4& transform, const Color& color, int ID = -1);
			void Flush();
			void NextBatch();
			void Begin(const glm::mat4& viewProjectionMatrix);
			void StartBatch();

		private:
			const uint32_t m_MaxVertices = 20000;
			const uint32_t m_MaxIndices = 20000;
			const uint32_t m_QuadVertexCount = 6;
			const uint32_t m_TriangleVertexCount = 6;

			std::shared_ptr<VertexArray> m_VertexArray;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
			std::shared_ptr<ShaderProgram> m_TextureShader;

			uint32_t m_VerticesCount = 0;
			uint32_t m_IndicesCount = 0;
			uint32_t m_TextureSlotIndex = 1; // 0 = white texture
			Vertex* m_VertexBufferAddress = nullptr; // This is the first position of the vertex buffer
			Vertex* m_VertexBufferPtr = nullptr; // this point to the last element added to the vertex buffer, IS NOT THE END OF THE BUFFER
			static const uint32_t m_MaxTextureSlots = 32;

			std::array<std::shared_ptr<Texture2D>, m_MaxTextureSlots> m_TextureSlots;
			std::vector<glm::vec2> m_CircleVertices;

			const glm::vec4 QuadVertexPositions[6] = {
				{-0.5f,-0.5f, 0.0f, 1.0f },
				{ 0.5f,-0.5f, 0.0f, 1.0f },
				{ 0.5f, 0.5f, 0.0f, 1.0f },
				{ 0.5f, 0.5f, 0.0f, 1.0f },
				{-0.5f, 0.5f, 0.0f, 1.0f},
				{-0.5f,-0.5f, 0.0f, 1.0f },
			};

			const glm::vec4 m_TriangleVertices[3] = {
				{-0.5f, -0.5f, 0.f, 1.f},
				{ 0.5f, -0.5f, 0.f, 1.f},
				{ 0.0f,  0.5f, 0.f, 1.f},
			};

			const glm::vec4 m_RectTriangleVertices[3] = {
				{-0.5f, -0.5f, 0.f, 1.f},
				{ 0.5f, -0.5f, 0.f, 1.f},
				{-0.5f,  0.5f, 0.f, 1.f},
			};

			const glm::vec2 m_QuadTextureCoords[6] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 1.0f, 1.0f },
				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
				{ 0.0f, 0.0f },
			};

			const glm::vec2 m_TriangleTextureCoords[6] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 1.0f, 1.0f },
			};

			const glm::vec2 m_RectTriangleTextureCoords[6] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 0.0f, 1.0f },
			};
		};

		/*Use line loop that use less vertices*/

		class WirePolygons
		{
		public:
			WirePolygons();
			~WirePolygons();

			void Draw(const std::vector<glm::vec2>& positions, const glm::mat4& transform, const Color& color, int ID = -1);
			void DrawLine(const std::pair<glm::vec2, glm::vec2>& positions, const Color& color, int layer = 0, int ID = -1);
			void DrawTriangle(const glm::mat4& transform, const Color& color, int layer = 0, int ID = -1);
			void DrawQuad(const glm::mat4& transform, const Color& color, int layer = 0, int ID = -1);
			void DrawRectTriangle(const glm::mat4& transform, const Color& color, int layer = 0, int ID = -1);
			void Flush();
			void NextBatch();
			void Begin(const glm::mat4& viewProjectionMatrix);
			void StartBatch();

		private:
			std::shared_ptr<VertexArray> m_VertexArray;
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<ShaderProgram> m_TextureShader;

			const uint32_t m_MaxVertices = 20000;
			static const uint32_t m_MaxTextureSlots = 32;
			uint32_t m_VerticesCount = 0;
			Vertex* m_VertexBufferAddress = nullptr; // This is the first position of the vertex buffer
			Vertex* m_VertexBufferPtr = nullptr; // this point to the last element added to the vertex buffer, IS NOT THE END OF THE BUFFER

			const glm::vec4 m_TriangleVertices[4] = {
				{-0.5f, -0.5f, 0.f, 1.f}, // 0
				{ 0.5f, -0.5f, 0.f, 1.f}, // 1
				{ 0.0f,  0.5f, 0.f, 1.f}, // 2
				{-0.5f, -0.5f, 0.f, 1.f}, // 0
			};

			const glm::vec4 m_RectTriangleVertices[4] = {
				{-0.5f, -0.5f, 0.f, 1.f}, // 0
				{ 0.5f, -0.5f, 0.f, 1.f}, // 1
				{-0.5f,  0.5f, 0.f, 1.f}, // 2
				{-0.5f, -0.5f, 0.f, 1.f}, // 0
			};

			const glm::vec4 QuadVertexPositions[5] = {
				{-0.5f,-0.5f, 0.0f, 1.0f }, // 0
				{ 0.5f,-0.5f, 0.0f, 1.0f }, // 1
				{ 0.5f, 0.5f, 0.0f, 1.0f }, // 2
				{-0.5f, 0.5f, 0.0f, 1.0f }, // 3
				{-0.5f,-0.5f, 0.0f, 1.0f }, // 0
			};

			const glm::vec2 m_QuadTextureCoords[4] = {
				{ 0.0f, 0.0f },
				{ 1.0f, 0.0f },
				{ 1.0f, 1.0f },
				{ 0.0f, 1.0f },
			};
		};
	}
}
