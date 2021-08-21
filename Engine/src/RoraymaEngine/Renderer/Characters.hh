#pragma once
#include <memory>
#include <unordered_map>
#include <string_view>

#include <glm/glm.hpp>

#include "Texture.hh"

namespace rym
{
    struct Character {
        std::shared_ptr<api::Texture2D> tex;
        glm::ivec2 size;      // Size of glyph
        glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
        unsigned int advance;   // Horizontal offset to advance to next glyph
    };

    class Characters
    {
    public:
        Characters(const std::string_view& path);
        ~Characters();
        const std::unordered_map<uint8_t, Character>& GetCharactersMap() { return m_CharactersMap; }
        const glm::ivec2& GetMaxSize() { return m_MaxSize; }

        const glm::ivec2& GetMaxBearing() { return m_MaxBearing; }
    private:
        std::unordered_map<uint8_t, Character> m_CharactersMap;
        glm::ivec2 m_MaxBearing = { 0, 0 };
        glm::ivec2 m_MaxSize = { 0, 0 };
        glm::ivec2 m_MinSize = { 0, 0 };
    };

}