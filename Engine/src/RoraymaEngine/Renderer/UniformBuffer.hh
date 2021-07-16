#pragma once
#include <cstdint>

namespace rym
{
	namespace api
	{
		class UniformBuffer
		{
		public:
			UniformBuffer(uint32_t size, uint32_t binding);
			~UniformBuffer();
			void SetData(const void* data, uint32_t size, uint32_t offset);
		private:
			uint32_t m_ID = 0;
		};
	}
}