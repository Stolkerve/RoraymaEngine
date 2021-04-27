#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include <initializer_list>

namespace rym
{
	namespace api
	{
		class FrameBuffer
		{
		public:
			FrameBuffer(uint32_t width, uint32_t height);
			~FrameBuffer();

			void Bind() const;
			void UnBind() const;

			uint32_t GetColorAttachment();
			void Resize(uint32_t width, uint32_t height);

			int ReadEntitys(int x, int y);

			void ClearEntitysAttachment();
		private:
			void CreateFrameBuffer();

			struct FrameBufferSize
			{
				uint32_t width;
				uint32_t height;
			}m_Size;

			uint32_t m_FrameBufferID;
			uint32_t m_ColorAttachment;
			uint32_t m_DepthStencilAttachment;
			uint32_t m_EntitysPickAttachment;
		};
	}
}