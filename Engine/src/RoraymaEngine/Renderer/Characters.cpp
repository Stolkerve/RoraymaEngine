#include "Characters.hh"
#include "../Core/Log.hh"

#include <filesystem>

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

rym::Characters::Characters(const std::string_view& path)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		RYM_ERROR("ERROR::FREETYPE: Could not init FreeType Library");

	FT_Face face;
	//RYM_INFO(std::filesystem::);
	if (FT_New_Face(ft, path.data(), 0, &face))
		RYM_ERROR("ERROR::FREETYPE: Failed to load font");
	else
	{
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
		// load 255 characters of ASCII set
		for (uint8_t c = 0; c < 0xff; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				RYM_ERROR("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			// generate texture
			constexpr api::TexConfig texConfig = {
				static_cast<int>(api::TextParam::LINEAR),
				static_cast<int>(api::TextParam::NEAREST),
				static_cast<int>(api::TextParam::CLAMP_TO_EDGE),
				static_cast<int>(api::TextParam::CLAMP_TO_EDGE),
				static_cast<int>(api::TextFormat::RED),
				static_cast<int>(api::TextInternalFormat::RGBA8)
			};

			Character character = {
				std::make_shared<api::Texture2D>(
					(uint32_t)face->glyph->bitmap.width,
					(uint32_t)face->glyph->bitmap.rows,
					(uint32_t*)face->glyph->bitmap.buffer,
					texConfig
				),
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			m_CharactersMap.insert({ c, character });

			m_MaxBearing.x = face->glyph->bitmap_left > m_MaxBearing.x ? face->glyph->bitmap_left : m_MaxBearing.x;
			m_MaxBearing.y = face->glyph->bitmap_top > m_MaxBearing.y ? face->glyph->bitmap_top : m_MaxBearing.y;

			m_MaxSize.x = face->glyph->bitmap.width > m_MaxSize.x ? face->glyph->bitmap.width : m_MaxSize.x;
			m_MaxSize.y = face->glyph->bitmap.rows > m_MaxSize.y ? face->glyph->bitmap.rows : m_MaxSize.y;

		}

		//glPixelStorei(GL_UNPACK_ALIGNMENT, 0); // disable byte-alignment restriction
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

rym::Characters::~Characters()
{

}
