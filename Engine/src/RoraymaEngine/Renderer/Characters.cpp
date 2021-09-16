#include "Characters.hh"

#include "../Core/Log.hh"
#include "../Utils/Base64.hh"
#include "Renderer.hh"

#include <filesystem>
#include <ft2build.h>
#include FT_FREETYPE_H

rym::Characters::Characters()
{}

rym::Characters::~Characters()
{

}

void rym::Characters::LoadFromFile(const std::string& path, float fontSize)
{
	m_Path = path;
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		RYM_ERROR("ERROR::FREETYPE: Could not init FreeType Library");

	FT_Face face;
	if (FT_New_Face(ft, path.data(), 0, &face))
		RYM_ERROR("ERROR::FREETYPE: Failed to load font");
	else
	{
		m_TextureAtlats = std::make_shared<api::Texture2D>();

		FT_Set_Pixel_Sizes(face, 0, fontSize);
		FT_GlyphSlot g = face->glyph;
		int w = 0;
		int h = 0;

		for (int i = 0; i < 255; i++)
		{
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			w += g->bitmap.width + 1;
			h = std::max(h, (int)g->bitmap.rows);
		}

		m_AtlatsWidth = w;
		m_AtlatsHeight = h;

		constexpr api::TexConfig texConfig = {
			static_cast<int>(api::TextParam::LINEAR),
			static_cast<int>(api::TextParam::NEAREST),
			static_cast<int>(api::TextParam::CLAMP_TO_EDGE),
			static_cast<int>(api::TextParam::CLAMP_TO_EDGE),
			static_cast<int>(api::TextFormat::RED),
			static_cast<int>(api::TextInternalFormat::RGBA8)
		};

		api::Renderer::PixelStorei(0x0CF5 /*unpack*/, 1);
		m_TextureAtlats->GenTexture(w, h, texConfig);

		int x = 0;
		int y = 0;
		for (uint8_t c = 0; c < 255; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				RYM_ERROR("Loading error");
				continue;
			}
			//glTextureSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			m_TextureAtlats->SubTexture(g->bitmap.buffer, x, y, g->bitmap.width, g->bitmap.rows, texConfig);

			Character character = {
				(float)x / (float)w,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			m_CharactersMap.insert({ c, character });

			m_MaxBearing.x = face->glyph->bitmap_left > m_MaxBearing.x ? face->glyph->bitmap_left : m_MaxBearing.x;
			m_MaxBearing.y = face->glyph->bitmap_top > m_MaxBearing.y ? face->glyph->bitmap_top : m_MaxBearing.y;

			m_MaxSize.x = face->glyph->bitmap.width > m_MaxSize.x ? face->glyph->bitmap.width : m_MaxSize.x;
			m_MaxSize.y = face->glyph->bitmap.rows > m_MaxSize.y ? face->glyph->bitmap.rows : m_MaxSize.y;
			x += g->bitmap.width + 1;
		}

		RYM_INFO("Generated a {0}x{1} ({2} kb) texture atlas", m_AtlatsWidth, m_AtlatsHeight, m_AtlatsWidth * m_AtlatsHeight / 1024);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void rym::Characters::LoadFromMemoryBase64(const std::string& source, float fontSize)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		RYM_ERROR("ERROR::FREETYPE: Could not init FreeType Library");

	FT_Face face;
	
	std::string sourceDecode;
	Base64::Decode(source, sourceDecode);
	if (FT_New_Memory_Face(ft, (unsigned char*)sourceDecode.data(), sourceDecode.size(), 0, &face))
		RYM_ERROR("ERROR::FREETYPE: Failed to load font");
	else
	{
		m_TextureAtlats = std::make_shared<api::Texture2D>();

		FT_Set_Pixel_Sizes(face, 0, fontSize);
		FT_GlyphSlot g = face->glyph;
		int w = 0;
		int h = 0;

		for (int i = 0; i < 255; i++)
		{
			if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				fprintf(stderr, "Loading character %c failed!\n", i);
				continue;
			}

			w += g->bitmap.width + 1;
			h = std::max(h, (int)g->bitmap.rows);
		}

		m_AtlatsWidth = w;
		m_AtlatsHeight = h;

		constexpr api::TexConfig texConfig = {
			static_cast<int>(api::TextParam::LINEAR),
			static_cast<int>(api::TextParam::NEAREST),
			static_cast<int>(api::TextParam::CLAMP_TO_EDGE),
			static_cast<int>(api::TextParam::CLAMP_TO_EDGE),
			static_cast<int>(api::TextFormat::RED),
			static_cast<int>(api::TextInternalFormat::RGBA8)
		};

		api::Renderer::PixelStorei(0x0CF5 /*unpack*/, 1);
		m_TextureAtlats->GenTexture(w, h, texConfig);

		int x = 0;
		int y = 0;
		for (uint8_t c = 0; c < 255; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				RYM_ERROR("Loading error");
				continue;
			}
			//glTextureSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
			m_TextureAtlats->SubTexture(g->bitmap.buffer, x, y, g->bitmap.width, g->bitmap.rows, texConfig);
			
			Character character = {
				(float)x / (float)w,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			m_CharactersMap.insert({ c, character });

			m_MaxBearing.x = face->glyph->bitmap_left > m_MaxBearing.x ? face->glyph->bitmap_left : m_MaxBearing.x;
			m_MaxBearing.y = face->glyph->bitmap_top > m_MaxBearing.y ? face->glyph->bitmap_top : m_MaxBearing.y;

			m_MaxSize.x = face->glyph->bitmap.width > m_MaxSize.x ? face->glyph->bitmap.width : m_MaxSize.x;
			m_MaxSize.y = face->glyph->bitmap.rows > m_MaxSize.y ? face->glyph->bitmap.rows : m_MaxSize.y;
			x += g->bitmap.width + 1;
		}

		RYM_INFO("Generated a {0}x{1} ({2} kb) texture atlas", m_AtlatsWidth, m_AtlatsHeight, m_AtlatsWidth * m_AtlatsHeight / 1024);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}
