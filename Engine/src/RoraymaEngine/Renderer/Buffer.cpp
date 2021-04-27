#include "Buffer.hh"
#include <glad/glad.h>

namespace rym
{
	namespace api
	{
		struct Vertex;
		/* Vertex buffer */
		VertexBuffer::VertexBuffer(uint32_t size)
		{
			glCreateBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		}

		VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
		{
			glCreateBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_STATIC_DRAW);
		}

		void VertexBuffer::Bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
		}

		void VertexBuffer::UnBind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		VertexBuffer::~VertexBuffer()
		{
			glDeleteBuffers(1, &m_ID);
		}

		void VertexBuffer::SendData(const void* data, uint32_t size)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
		}

		/* Index buffer */
		// TODO: add std::optional for uint16 and uint32
		IndexBuffer::IndexBuffer(uint32_t size)
		{
			glCreateBuffers(1, &m_ID);
			glBindBuffer(GL_ARRAY_BUFFER, m_ID);
			glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
		}

		IndexBuffer::IndexBuffer(uint32_t size, uint32_t* indices) :
			m_Size(size)
		{
			if (indices)
			{
				glCreateBuffers(1, &m_ID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32_t), indices, GL_STATIC_DRAW);
			}
			else
			{
				glCreateBuffers(1, &m_ID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
			}
		}

		IndexBuffer::~IndexBuffer()
		{
			glDeleteBuffers(1, &m_ID);
		}

		void IndexBuffer::SendData(const void* data, uint32_t size)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
		}

		void IndexBuffer::Bind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
		}

		void IndexBuffer::UnBind() const
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}
