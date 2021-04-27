#pragma once
#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include "../Core/Assert.hh"

namespace rym
{
	namespace api
	{
		enum class AttribDataType {
			Float, Float2, Float3, Float4,
			Mat3, Mat4,
			Int, Int2, Int3, Int4
		};

		inline static uint32_t DataTypeToSize(const AttribDataType& type)
		{
			switch (type)
			{
			case AttribDataType::Float:		return 4;
			case AttribDataType::Float2:	return 4 * 2;
			case AttribDataType::Float3:	return 4 * 3;
			case AttribDataType::Float4:	return 4 * 4;
			case AttribDataType::Mat3:		return 4 * 3 * 3;
			case AttribDataType::Mat4:		return 4 * 4 * 4;
			case AttribDataType::Int:		return 4;
			case AttribDataType::Int2:		return 4 * 2;
			case AttribDataType::Int3:		return 4 * 3;
			case AttribDataType::Int4:		return 4 * 4;
			default:
				RYM_CORE_ERROR("This data type do not exit");
				return 0;
			}
		}

		class ElementsAttrib
		{
		public:
			ElementsAttrib(const AttribDataType& type, const std::string& name, bool normalize = false) :
				size(DataTypeToSize(type)), offset(0), type(type), name(name), normalize(normalize)
			{}

			inline uint32_t GetDataTypeCount() const
			{
				switch (type)
				{
				case AttribDataType::Float:		return 1;
				case AttribDataType::Float2:	return 2;
				case AttribDataType::Float3:	return 3;
				case AttribDataType::Float4:	return 4;
				case AttribDataType::Mat3:		return 3 * 3;
				case AttribDataType::Mat4:		return 4 * 4;
				case AttribDataType::Int:		return 1;
				case AttribDataType::Int2:		return 2;
				case AttribDataType::Int3:		return 3;
				case AttribDataType::Int4:		return 4;
				default:
					RYM_CORE_ERROR("This data type do no exist");
					return 0;
				}
			}
			inline uint32_t GetGLDataType() const
			{
				// GL_FLOAT = 0x1406
				// GL_INT = 0x1404
				switch (type)
				{
				case AttribDataType::Float:		return 0x1406;
				case AttribDataType::Float2:	return 0x1406;
				case AttribDataType::Float3:	return 0x1406;
				case AttribDataType::Float4:	return 0x1406;
				case AttribDataType::Mat3:		return 0x1406;
				case AttribDataType::Mat4:		return 0x1406;
				case AttribDataType::Int:		return 0x1404;
				case AttribDataType::Int2:		return 0x1404;
				case AttribDataType::Int3:		return 0x1404;
				case AttribDataType::Int4:		return 0x1404;
				default:
					RYM_CORE_ERROR("This gl data type do not exist");
					return 0x0;
				}
			}

		public:
			uint32_t size, offset;
			AttribDataType type;
			bool normalize;
			std::string name;
		};

		class BufferAttrib
		{
		public:
			BufferAttrib() = default;
			BufferAttrib(const std::initializer_list<ElementsAttrib>& elements) :
				m_Elements(elements), m_Stride(0)
			{
				uint32_t tempOffset = 0;
				for (auto& e : m_Elements)
				{
					e.offset = tempOffset;
					tempOffset += e.size;
					m_Stride += e.size;
				}
			}

			inline auto GetElements() const { return m_Elements; }
			inline auto GetStride() const { return m_Stride; }

			std::vector<ElementsAttrib>::iterator begin() { return m_Elements.begin(); }
			std::vector<ElementsAttrib>::iterator end() { return m_Elements.end(); }
			std::vector<ElementsAttrib>::const_iterator begin() const { return m_Elements.begin(); }
			std::vector<ElementsAttrib>::const_iterator end() const { return m_Elements.end(); }

		private:
			std::vector<ElementsAttrib> m_Elements;
			uint32_t m_Stride;
		};

		class Buffer
		{
		public:
			virtual void Bind() const = 0;
			virtual void UnBind() const = 0;

		protected:
			uint32_t m_ID;
		};

		class VertexBuffer : public Buffer
		{
		public:
			VertexBuffer() = default;
			VertexBuffer(uint32_t size);
			VertexBuffer(float* vertices, uint32_t size);
			~VertexBuffer();

			void SendData(const void* data, uint32_t size);
			void SetLayout(const BufferAttrib& attrib) { m_Attrib = attrib; }
			const BufferAttrib& getLayout() const { return m_Attrib; }
			virtual void Bind() const override;
			virtual void UnBind() const override;

		private:
			BufferAttrib m_Attrib;
		};

		class IndexBuffer : public Buffer
		{
		public:
			// \If indices keep nullptr the constructor will define GL_DYNAMIC_DRAW on glBufferData 
			IndexBuffer(uint32_t size);
			IndexBuffer(uint32_t size, uint32_t* indices);
			~IndexBuffer();

			inline uint32_t GetSize() const { return m_Size; }
			void SendData(const void* data, uint32_t size);

			virtual void Bind() const override;
			virtual void UnBind() const override;
		private:
			uint32_t m_Size;
		};
	}
}
