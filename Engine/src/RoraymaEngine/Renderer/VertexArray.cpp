#include "VertexArray.hh"
#include <glad/glad.h>

namespace rym
{
	namespace api
	{
		VertexArray::VertexArray()
		{
			glCreateVertexArrays(1, &m_ID);
			glBindVertexArray(m_ID);
		}

		VertexArray::~VertexArray()
		{
			glDeleteVertexArrays(1, &m_ID);
		}

		void VertexArray::Bind() const
		{
			glBindVertexArray(m_ID);
		}

		void VertexArray::UnBind() const
		{
			glBindVertexArray(NULL);
		}

		void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
		{
			// assert vertexBuffer->getLayout().getElements().size() !=0 
			glBindVertexArray(m_ID);
			vertexBuffer->Bind();

			const auto& attrib = vertexBuffer->getLayout();
			uint32_t index = 0;
			for (const auto& e : attrib)
			{
				glEnableVertexAttribArray(index);
				if (e.GetGLDataType() != GL_INT)
					glVertexAttribPointer(index, e.GetDataTypeCount(), e.GetGLDataType(), e.normalize, attrib.GetStride(), reinterpret_cast<void*>(e.offset));
				else
					glVertexAttribIPointer(index, e.GetDataTypeCount(), e.GetGLDataType(), attrib.GetStride(), reinterpret_cast<void*>(e.offset));
				index++;
			}

			m_VertexBuffer = vertexBuffer;
		}

		void VertexArray::AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
		{
			glBindVertexArray(m_ID);
			indexBuffer->Bind();
			m_IndexBuffer = indexBuffer;
		}

	}
}

