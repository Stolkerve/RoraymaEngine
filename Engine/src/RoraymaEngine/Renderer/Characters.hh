#pragma once
#include <memory>
#include <unordered_map>
#include <string_view>

#include <glm/glm.hpp>

#include "Texture.hh"

namespace rym
{
    //struct Character {
    //    std::shared_ptr<api::Texture2D> tex;
    //    glm::ivec2 size;      // Size of glyph
    //    glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
    //    unsigned int advance;   // Horizontal offset to advance to next glyph
    //};
    
    struct Character {
        float tx;
        glm::ivec2 size;      // Size of glyph
        glm::ivec2 bearing;   // Offset from baseline to left/top of glyph
        unsigned int advance;   // Horizontal offset to advance to next glyph
    };

    class Characters
    {
        
    public:
        Characters();
        ~Characters();
        void LoadFromFile(const std::string& path, float fontSize);
        void LoadFromMemoryBase64(const std::string& source, float fontSize);
        const std::unordered_map<uint8_t, Character>& GetCharactersMap() const { return m_CharactersMap; }
        const glm::ivec2& GetMaxSize() const { return m_MaxSize; }
        const glm::ivec2& GetMaxBearing() const { return m_MaxBearing; }
        const std::string& GetPath() const { return m_Path; }
        const std::shared_ptr<api::Texture2D>& GetTextAtlats() const { return m_TextureAtlats; }
        const int GetAtlatsWidth() const { return m_AtlatsWidth; }
        const int GetAtlatsHeight() const { return m_AtlatsHeight; }
    private:
        std::shared_ptr<api::Texture2D> m_TextureAtlats;
        int m_AtlatsWidth;
        int m_AtlatsHeight;
        std::unordered_map<uint8_t, Character> m_CharactersMap;
        glm::ivec2 m_MaxBearing = { 0, 0 };
        glm::ivec2 m_MaxSize = { 0, 0 };
        glm::ivec2 m_MinSize = { 0, 0 };
        std::string m_Path;
    };

}