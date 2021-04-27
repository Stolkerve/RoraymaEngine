#pragma once

#include <cstdint>
#include <string>
#include <array>

namespace rym
{
	namespace api
	{
		class Texture
		{
		public:

			virtual uint32_t GetWidth() const = 0;
			virtual uint32_t GetHeight() const = 0;
			virtual uint32_t GetID() const = 0;

			virtual void Bind(uint32_t slot) const = 0;
			virtual void UnBind() const = 0;

			virtual bool operator==(const Texture& t) = 0;
		protected:
			uint32_t m_ID;
		};

		class Texture2D : public Texture
		{
		public:
			Texture2D(const std::string_view& path);
			Texture2D(uint32_t width, uint32_t height);
			~Texture2D();

			virtual uint32_t GetWidth() const override { return m_Width; };
			virtual uint32_t GetHeight() const override { return m_Height; };
			virtual uint32_t GetID() const override { return m_ID; }

			virtual void Bind(uint32_t slot) const override;
			virtual void UnBind() const override;
			virtual bool operator==(const Texture& t) override;

			void SetData(uint32_t* data);

			uint32_t GetID() { return m_ID; }
			std::string GetPath() { return m_Path; };
		private:
			std::string m_Path = "null";
			uint32_t m_Width, m_Height;
			uint32_t m_Channel;
		};
	}
}