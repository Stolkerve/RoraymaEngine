#include "Texture.hh"

#include <glad/glad.h>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../Core/Log.hh"

#if 1
namespace rym
{
	namespace api
	{
		Texture2D::Texture2D(const std::string_view& path) :
			m_Path(path), m_Width(0), m_Height(0), m_Channel(0)
		{
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

				glTextureParameteri(m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTextureParameteri(m_ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				glTextureParameteri(m_ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(m_ID, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

		Texture2D::Texture2D(uint32_t width, uint32_t height) :
			m_Width(width), m_Height(height)
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &m_ID);
			glTextureStorage2D(m_ID, 1, GL_RGBA8, m_Width, m_Height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		void Texture2D::SetData(uint32_t* data)
		{
			glTextureSubImage2D(m_ID, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		Texture2D::~Texture2D()
		{
			glDeleteTextures(1, &m_ID);
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
