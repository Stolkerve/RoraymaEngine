#pragma once

#include "Buffer.hh"
#include <memory>

namespace rym
{
	namespace api
	{
		class VertexArray : public Buffer
		{
		public:
			VertexArray();
			~VertexArray();

			virtual void Bind() const override;
			virtual void UnBind() const override;

			void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
			void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

			const std::shared_ptr<IndexBuffer>& GetIndexBuffer() { return m_IndexBuffer; }
			const std::shared_ptr<VertexBuffer>& GetVertexBuffer() { return m_VertexBuffer; }

		private:
			std::shared_ptr<VertexBuffer> m_VertexBuffer;
			std::shared_ptr<IndexBuffer> m_IndexBuffer;
		};
	}
}

