#include "BatchRendering.hh"
#include "../Scene/AssetsManager.hh"
#include "../Utils/Triangulate.hh"

#include <future>
#include <unordered_map>

namespace rym
{
	namespace api
	{
		BatchStatistics& BatchStatistics::Get()
		{
			static BatchStatistics instance;
			return instance;
		}
		void BatchStatistics::reset()
		{
			DrawCalls = 0;
			NumOfVertices = 0;
		}

		static std::unordered_map<std::string, std::vector<glm::vec2>> s_Meshes;

		Polygons::Polygons()
		{
			m_VertexBufferAddress = new Vertex[m_MaxVertices];

			m_VertexArray = std::make_shared<VertexArray>();

			m_VertexBuffer = std::make_shared<VertexBuffer>(m_MaxVertices * uint32_t(sizeof(Vertex)));
			m_VertexBuffer->SetLayout({
				{  api::AttribDataType::Float3, "a_Positions" },
				{  api::AttribDataType::Float4, "a_Colors" },
				{  api::AttribDataType::Float2, "a_TexCoords" },
				{  api::AttribDataType::Float, "a_TexIndex" },
				{  api::AttribDataType::Float, "a_Layer" },
				{  api::AttribDataType::Int, "a_EntityID" },
				});
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			// Init texture slots

			// Set first texture slot to 0
			m_TextureSlots[0] = AssetsManager::GetTexture("WhiteTexture");

			std::vector<glm::vec2> cirVertives;

			size_t segments = 30;
			cirVertives.reserve(segments);
			for (size_t i = 0; i < segments; i++)
			{
				float theta = 2.0f * 3.1415926f * float(i) / float(segments);
				cirVertives.push_back({ 0.5f * glm::cos(theta), 0.5f * glm::sin(theta) });
			}
			m_CircleVertices.reserve(cirVertives.size());
			Triangulate::Process(cirVertives, m_CircleVertices);
			auto a = m_CircleVertices.data();
		}

		Polygons::~Polygons()
		{
			delete[] m_VertexBufferAddress;
		}

		void Polygons::DrawQuad(const glm::mat4& transform, const Color& color, int layer, int ID)
		{
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount + m_QuadVertexCount >= m_MaxVertices)
				NextBatch();

			for (uint32_t i = 0; i < m_QuadVertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * QuadVertexPositions[i];
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = float(layer);
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += m_QuadVertexCount;
			BatchStatistics::Get().NumOfVertices += m_QuadVertexCount;
		}

		void Polygons::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const Color& color, int layer, int ID)
		{
			if (m_VerticesCount + m_QuadVertexCount >= m_MaxVertices)
				NextBatch();

			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
			{
				if (*m_TextureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (m_TextureSlotIndex >= m_MaxTextureSlots)
					NextBatch();

				textureIndex = (float)m_TextureSlotIndex;
				m_TextureSlots[m_TextureSlotIndex] = texture;
				m_TextureSlotIndex++;
			}

			for (size_t i = 0; i < m_QuadVertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * QuadVertexPositions[i];
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = float(layer);
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += m_QuadVertexCount;
			BatchStatistics::Get().NumOfVertices += m_QuadVertexCount;
		}

		void Polygons::DrawText_(const glm::vec4* verticesPos, const std::shared_ptr<Texture2D>& texture, const Color& color, int layer, int ID)
		{
			if (m_VerticesCount + m_QuadVertexCount >= m_MaxVertices)
				NextBatch();

			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
			{
				if (*m_TextureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (m_TextureSlotIndex >= m_MaxTextureSlots)
					NextBatch();

				textureIndex = (float)m_TextureSlotIndex;
				m_TextureSlots[m_TextureSlotIndex] = texture;
				m_TextureSlotIndex++;
			}

			for (size_t i = 0; i < m_QuadVertexCount; i++)
			{
				m_VertexBufferPtr->Position = glm::vec4(verticesPos[i].x, verticesPos[i].y, 0.f, 1.f);
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = glm::vec2(verticesPos[i].z, verticesPos[i].w);
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = float(layer);
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += m_QuadVertexCount;
			BatchStatistics::Get().NumOfVertices += m_QuadVertexCount;
		}

		void Polygons::DrawQuad(const glm::vec4* verticesPos, const glm::vec2* uv, const std::shared_ptr<Texture2D>& texture, const Color& color, int layer, int ID)
		{
			if (m_VerticesCount + m_QuadVertexCount >= m_MaxVertices)
				NextBatch();

			float textureIndex = 0.0f;
			for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
			{
				if (*m_TextureSlots[i] == *texture)
				{
					textureIndex = (float)i;
					break;
				}
			}

			if (textureIndex == 0.0f)
			{
				if (m_TextureSlotIndex >= m_MaxTextureSlots)
					NextBatch();

				textureIndex = (float)m_TextureSlotIndex;
				m_TextureSlots[m_TextureSlotIndex] = texture;
				m_TextureSlotIndex++;
			}

			for (size_t i = 0; i < m_QuadVertexCount; i++)
			{
				m_VertexBufferPtr->Position = glm::vec4(verticesPos[i].x, verticesPos[i].y, 0.f, 1.f);;
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = uv[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = float(layer);
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += m_QuadVertexCount;
			BatchStatistics::Get().NumOfVertices += m_QuadVertexCount;
		}

		void Polygons::DrawCircle(const glm::mat4& transform, const Color& color, int layer, int ID)
		{
			size_t vertexCount = m_CircleVertices.size();
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount + vertexCount >= m_MaxVertices)
				NextBatch();

			for (uint32_t i = 0; i < vertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * glm::vec4(m_CircleVertices[i], 0.f, 1.f);
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = float(layer);
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			size_t i = 0;
				//std::lock_guard<std::mutex> lock{ myMutex };

			/*
			std::for_each(std::execution::par, m_CircleVertices.begin(), m_CircleVertices.end(), [&](const glm::vec2& v) {
				std::lock_guard<std::mutex> lock{ myMutex };
				m_VertexBufferPtr->Position = transform * glm::vec4(m_CircleVertices[i], 0.f, 1.f);
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = 0.f;
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
				i++;
				});
			*/

			m_VerticesCount += vertexCount;
			BatchStatistics::Get().NumOfVertices += vertexCount;
		}

		void Polygons::DrawPolygon(const std::vector<glm::vec2>& vertices, const glm::mat4& transform, const Color& color, int layer, int ID)
		{
			std::vector<glm::vec2> result; // TODO: reserve space
			Triangulate::Process(vertices, result);

			size_t vertexCount = result.size();
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount + vertexCount >= m_MaxVertices)
				NextBatch();

			for (size_t i = 0; i < vertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * glm::vec4(result[i], 0.f, 1.f);
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = layer;
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += vertexCount;
			BatchStatistics::Get().NumOfVertices += vertexCount;
		}

		void Polygons::DrawTriangle(const glm::mat4& transform, const Color& color, int ID)
		{
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount >= m_MaxVertices)
				NextBatch();

			for (size_t i = 0; i < m_TriangleVertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * m_TriangleVertices[i];
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_TriangleTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = 0.f;
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += m_TriangleVertexCount;
			BatchStatistics::Get().NumOfVertices += m_TriangleVertexCount;
		}

		void Polygons::DrawRectTriangle(const glm::mat4& transform, const Color& color, int ID)
		{
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount >= m_MaxVertices)
				NextBatch();

			for (size_t i = 0; i < m_TriangleVertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * m_RectTriangleVertices[i];
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_RectTriangleTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = 0.f;
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += m_TriangleVertexCount;
			BatchStatistics::Get().NumOfVertices += m_TriangleVertexCount;
		}

		void Polygons::Flush()
		{
			if (m_VerticesCount == 0)
				return; // Nothing to draw

			uint32_t dataSize = (uint32_t)((uint8_t*)m_VertexBufferPtr - (uint8_t*)m_VertexBufferAddress);
			m_VertexBuffer->SendData(m_VertexBufferAddress, dataSize);

			// Bind textures
			for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
				m_TextureSlots[i]->Bind(i);

			Renderer::DrawArrays(m_VertexArray, 0, m_VerticesCount); // GL_TRIANGLES
			BatchStatistics::Get().DrawCalls++;
		}

		void Polygons::NextBatch()
		{
			Flush();
			StartBatch();
		}

		void Polygons::Begin(const glm::mat4& viewProjectionMatrix)
		{
			//m_TextureShader->Bind();
			//m_TextureShader->SetUniformMat4f("u_ViewProjection", viewProjectionMatrix);
			BatchStatistics::Get().reset();

			StartBatch();
		}

		void Polygons::StartBatch()
		{
			m_VerticesCount = 0;
			m_VertexBufferPtr = m_VertexBufferAddress;
			m_TextureSlotIndex = 1;
		}

		WirePolygons::WirePolygons()
		{
			m_VertexBufferAddress = new Vertex[m_MaxVertices];

			m_VertexArray = std::make_shared<VertexArray>();

			m_VertexBuffer = std::make_shared<VertexBuffer>(m_MaxVertices * uint32_t(sizeof(Vertex)));
			m_VertexBuffer->SetLayout({
				{  api::AttribDataType::Float3, "a_Positions" },
				{  api::AttribDataType::Float4, "a_Colors" },
				{  api::AttribDataType::Float2, "a_TexCoords" },
				{  api::AttribDataType::Float, "a_TexIndex" },
				{  api::AttribDataType::Float, "a_Layer" },
				{  api::AttribDataType::Int, "a_EntityID" },
				});
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);

			// Init texture slots
			std::array<int32_t, m_MaxTextureSlots> samplers{};
			std::iota(samplers.begin(), samplers.end(), 0);

			// Load shader
			//AssetsManager::PushShader("EditorShader", "assets/shaders/EditorShader.glsl");
			//m_TextureShader = AssetsManager::GetShader("TextureShader");
			//m_TextureShader->Bind();
			//m_TextureShader->SetUniformIntArray("u_Textures", samplers.data(), m_MaxTextureSlots);
		}

		WirePolygons::~WirePolygons()
		{
			delete[] m_VertexBufferAddress;
		}

		void WirePolygons::Draw(const std::vector<glm::vec2>& positions, const glm::mat4& transform, const Color& color, int ID)
		{
			/*
			//uint32_t vertexCount = positions.size() * 2;
			uint32_t vertexCount = result.size();
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount >= m_MaxVertices)
				NextBatch();

			for (uint32_t i = 0; i < vertexCount; i++)
			{
				m_VertexBufferPtr->Position = transform * glm::vec4(result[i], 0.f, 1.f);
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += vertexCount;
			BatchStatistics::Get().NumOfVertices += vertexCount;
			*/
		}

		void WirePolygons::DrawLine(const std::pair<glm::vec2, glm::vec2>& positions, const Color& color, int layer, int ID)
		{
			auto& [p1, p2] = positions;
			glm::vec4 tmpArray[2] = { glm::vec4(p1, 0.f, 1.f), glm::vec4(p2, 0.f, 1.f) };
			uint32_t vertexCount = 2;
			constexpr float textureIndex = 0.0f; // White Texture

			if (m_VerticesCount >= m_MaxVertices)
				NextBatch();

			for (uint32_t i = 0; i < vertexCount; i++)
			{
				m_VertexBufferPtr->Position = tmpArray[i];
				m_VertexBufferPtr->Color = color.GetColor();
				m_VertexBufferPtr->TexCoord = m_QuadTextureCoords[i];
				m_VertexBufferPtr->TexIndex = textureIndex;
				m_VertexBufferPtr->Layer = float(layer);
				m_VertexBufferPtr->EntityID = ID;
				m_VertexBufferPtr++;
			}

			m_VerticesCount += vertexCount;
			BatchStatistics::Get().NumOfVertices += vertexCount;
		}

		void WirePolygons::DrawQuad(const glm::mat4& transform, const Color& color, int layer, int ID)
		{
			const auto l1 = transform * glm::vec4(-0.5f,-0.5f, 0.f, 1.f);
			const auto l2 = transform * glm::vec4( 0.5f,-0.5f, 0.f, 1.f);
			const auto l3 = transform * glm::vec4( 0.5f, 0.5f, 0.f, 1.f);
			const auto l4 = transform * glm::vec4(-0.5f, 0.5f, 0.f, 1.f);

			DrawLine({ {l1}, {l2} }, color, layer, ID);
			DrawLine({ {l2}, {l3} }, color, layer, ID);
			DrawLine({ {l3}, {l4} }, color, layer, ID);
			DrawLine({ {l4}, {l1} }, color, layer, ID);
		}

		void WirePolygons::Flush()
		{
			if (m_VerticesCount == 0)
				return; // Nothing to draw

			uint32_t dataSize = (uint32_t)((uint8_t*)m_VertexBufferPtr - (uint8_t*)m_VertexBufferAddress);
			m_VertexBuffer->SendData(m_VertexBufferAddress, dataSize);

			// Bind texture
			AssetsManager::GetTexture("WhiteTexture")->Bind(0);

			Renderer::DrawArrays(m_VertexArray, 0, m_VerticesCount, 0x0001);
			BatchStatistics::Get().DrawCalls++;
		}

		void WirePolygons::NextBatch()
		{
			Flush();
			StartBatch();
		}

		void WirePolygons::Begin(const glm::mat4& viewProjectionMatrix)
		{
			//m_TextureShader->Bind();
			//m_TextureShader->SetUniformMat4f("u_ViewProjection", viewProjectionMatrix);
			BatchStatistics::Get().reset();

			StartBatch();
		}

		void WirePolygons::StartBatch()
		{
			m_VerticesCount = 0;
			m_VertexBufferPtr = m_VertexBufferAddress;
		}
}
}
