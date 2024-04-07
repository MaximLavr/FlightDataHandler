#pragma once

#include "texture.h"
#include "../../lib/freetype-2.10.0/freetype-gl/freetype-gl.h"
#include <string>
#include <ft2build.h>
#include <freetype/freetype.h>
#include "../maths/vec2.h"
#include <cstddef>


namespace sparky { namespace graphics {

    class Font
    {
    private:
        ftgl::texture_atlas_t* m_FTAtlas;
        ftgl::texture_font_t* m_FTFont;
        unsigned int m_Size;
        maths::vec2 m_Scale;
        std::string m_Name;
        std::string m_Filename;
    public:
        Font(std::string name, std::string filename, int size);
        ~Font() {
            delete m_FTAtlas;
            delete m_FTFont;
        }
        void setScale(float x, float y);

        inline ftgl::texture_font_t* getFTFont() const { return m_FTFont; }

        inline const unsigned int getID() const { return m_FTAtlas->id; }
        inline const maths::vec2& getScale() const { return m_Scale; }
        inline const std::string& getName() { return m_Name; }
        inline const std::string& getFilename() { return m_Filename; }
        //inline const int getSize() { return m_Size; }

        maths::vec2 GetOffsets(const std::string& text) const;
        float GetWidth(const std::string& text) const;
        float GetHeight(const std::string& text) const;
        maths::vec2 GetSize(const std::string& text) const;

        maths::Rectangle GetBounds(const std::string& text) const;

        Texture* GetTexture() const;

        float getWidth();
    };

} }