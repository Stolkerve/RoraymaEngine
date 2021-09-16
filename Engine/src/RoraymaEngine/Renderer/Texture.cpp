#include "Texture.hh"

#include <glad/glad.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../Core/Log.hh"
#include "../Utils/Base64.hh"

#if 1
namespace rym
{
	namespace api
	{
		//Texture2D::Texture2D(const std::string_view& path, TexConfig config) :
		//	m_Path(path), m_Width(0), m_Height(0), m_Channel(0)
		//{
		//	stbi_set_flip_vertically_on_load(true);

		//	int width, height, channel;
		//	stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channel, 0);
		//	m_Width = width; m_Height = height; m_Channel = channel;
		//	if (data)
		//	{
		//		if (m_Channel == 3)
		//		{
		//			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		//			glTextureStorage2D(m_ID, 1, GL_RGB8, m_Width, m_Height);
		//		}
		//		if (m_Channel == 4)
		//		{
		//			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		//			glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
		//		}

		//		glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, config.minFilter);
		//		glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, config.magFilter);
		//		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, config.wrapS);
		//		glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, config.wrapT);

		//		switch (m_Channel)
		//		{
		//		case 3:
		//			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
		//			break;
		//		case 4:
		//			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		//			break;
		//		default: // Red channel
		//			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RED, GL_UNSIGNED_BYTE, data);
		//			break;
		//		}
		//		stbi_image_free(data);
		//	}
		//	else
		//	{
		//		RYM_CORE_ERROR("ERROR::COULD::NOT::OPEN::TEXTURE: {0}", path);
		//	}
		//}

		//Texture2D::Texture2D(uint32_t width, uint32_t height, uint32_t* data, TexConfig config) :
		//	m_Width(width), m_Height(height)
		//{
		//	glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
		//	glTextureStorage2D(m_ID, 1, config.internalFormat, m_Width, m_Height);

		//	glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, config.minFilter);
		//	glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, config.magFilter);
		//	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, config.wrapS);
		//	glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, config.wrapT);

		//	glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, config.format, GL_UNSIGNED_BYTE, data);
		//}

		Texture2D::~Texture2D()
		{
			glDeleteTextures(1, &m_ID);
		}

		void Texture2D::GenTexture(uint32_t width, uint32_t height, TexConfig config)
		{
			m_Width = width; m_Height = height;
			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
			glTextureStorage2D(m_ID, 1, config.internalFormat, m_Width, m_Height);

			glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, config.minFilter);
			glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, config.magFilter);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, config.wrapS);
			glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, config.wrapT);
		}

		void Texture2D::SubTexture(void* data, int offsetX, int offsetY, uint32_t width, uint32_t height, TexConfig config)
		{
			glTextureSubImage2D(m_ID, 0, offsetX, offsetY, width, height, config.format, GL_UNSIGNED_BYTE, data);
		}

		void Texture2D::LoadFromFile(const std::string& path, TexConfig config)
		{
			m_Path = path; m_Width = 0; m_Height = 0; m_Channel = 0;
			stbi_set_flip_vertically_on_load(true);

			int width, height, channel;
			stbi_uc* data = stbi_load(m_Path.c_str(), &width, &height, &channel, 0);
			m_Width = width; m_Height = height; m_Channel = channel;
			if (data)
			{
				if (m_Channel == 3)
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
					glTextureStorage2D(m_ID, 1, GL_RGB8, m_Width, m_Height);
				}
				if (m_Channel == 4)
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
					glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
				}

				glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, config.minFilter);
				glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, config.magFilter);
				glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, config.wrapS);
				glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, config.wrapT);

				switch (m_Channel)
				{
				case 3:
					glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
					break;
				case 4:
					glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
					break;
				default: // Red channel
					glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RED, GL_UNSIGNED_BYTE, data);
					break;
				}
				stbi_image_free(data);
			}
			else
			{
				RYM_CORE_ERROR("ERROR::COULD::NOT::OPEN::TEXTURE: {0}", path);
			}
		}

		void Texture2D::LoadFromMemory(uint32_t width, uint32_t height, uint32_t* data, TexConfig config)
		{
			GenTexture(width, height, config);
			SubTexture(data, 0, 0, width, height, config);
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, config.format, GL_UNSIGNED_BYTE, data);
		}

		void Texture2D::LoadFromMemoryBase64(const std::string& source, TexConfig config)
		{
			m_Path = "source"; m_Width = 0; m_Height = 0; m_Channel = 0;
			stbi_set_flip_vertically_on_load(true);

			std::string decodeSource;
			Base64::Decode(source, decodeSource);

			int width, height, channel;
			stbi_uc* data = stbi_load_from_memory((unsigned char*)decodeSource.data(), source.size(), &width, &height, &channel, 0);

			m_Width = width; m_Height = height; m_Channel = channel;
			if (data)
			{
				if (m_Channel == 3)
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
					glTextureStorage2D(m_ID, 1, GL_RGB8, m_Width, m_Height);
				}
				if (m_Channel == 4)
				{
					glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
					glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);
				}

				glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, config.minFilter);
				glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, config.magFilter);
				glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, config.wrapS);
				glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, config.wrapT);

				switch (m_Channel)
				{
				case 3:
					glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);
					break;
				case 4:
					glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
					break;
				default: // Red channel
					glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RED, GL_UNSIGNED_BYTE, data);
					break;
				}
				stbi_image_free(data);
			}
			else
			{
				RYM_CORE_ERROR("ERROR::COULD::NOT::OPEN::TEXTURE: {0}", "source");
			}
		}

		void Texture2D::Bind(uint32_t slot) const
		{
			glBindTextureUnit(slot, m_ID);
			//glBindTexture(GL_TEXTURE_2D, m_ID);
			//glActiveTexture(GL_TEXTURE0 + slot);
		}

		void Texture2D::UnBind() const
		{
		}

		bool Texture2D::operator==(const Texture& t)
		{
			return m_ID == ((Texture2D&)t).m_ID;
		}
	}
}
#endif
