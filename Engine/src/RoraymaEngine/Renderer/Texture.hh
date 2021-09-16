#pragma once

#include <cstdint>
#include <string>
#include <array>

namespace rym
{
	namespace api
	{
		enum class TextParam
		{
			LINEAR = 0x2601,
			CLAMP_TO_EDGE = 0x812F,
			NEAREST = 0x2600,
			REPEAT = 0x2901
		};

		enum class TextFormat
		{
			RGBA = 0x1908,
			RGB = 0x1907,
			RED = 0x1903
		};

		enum class TextInternalFormat
		{
			RGBA8 = 0x8058,
			RGB8 = 0x8051,
			RED8 = 0x8229
		};

		struct TexConfig
		{
			int minFilter = static_cast<int>(TextParam::LINEAR);
			int magFilter = static_cast<int>(TextParam::NEAREST);
			int wrapS = static_cast<int>(TextParam::REPEAT);
			int wrapT = static_cast<int>(TextParam::REPEAT);

			int format = static_cast<int>(TextFormat::RGBA);
			int internalFormat = static_cast<int>(TextInternalFormat::RGBA8);
		};

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
			Texture2D() = default;
			//Texture2D(const std::string_view& path, TexConfig config = {});
			//Texture2D(uint32_t width, uint32_t height, uint32_t* data, TexConfig config = {});
			~Texture2D();
			
			void GenTexture(uint32_t width, uint32_t height, TexConfig config);
			void SubTexture(void* data, int offsetX, int offsetY, uint32_t width, uint32_t height,TexConfig config);
			void LoadFromFile(const std::string& path, TexConfig config = {});
			void LoadFromMemory(uint32_t width, uint32_t height, uint32_t* data, TexConfig config = {});
			void LoadFromMemoryBase64(const std::string& source, TexConfig config = {});

			virtual uint32_t GetWidth() const override { return m_Width; };
			virtual uint32_t GetHeight() const override { return m_Height; };
			virtual uint32_t GetID() const override { return m_ID; }

			virtual void Bind(uint32_t slot) const override;
			virtual void UnBind() const override;
			virtual bool operator==(const Texture& t) override;

			uint32_t GetID() { return m_ID; }
			std::string GetPath() { return m_Path; };
		private:
			std::string m_Path = "null";
			uint32_t m_Width, m_Height;
			uint32_t m_Channel;
		};
	}
}