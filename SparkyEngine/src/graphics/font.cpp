#include "font.h"

namespace sparky { namespace graphics {

    Font::Font(std::string name, std::string filename, int size)
        : m_Name(name), m_Filename(filename), m_Size(size), m_Scale(maths::vec2(1, 1))
    {
        m_FTAtlas = ftgl::texture_atlas_new(512, 512, 4);
        m_FTFont = ftgl::texture_font_new_from_file(m_FTAtlas,m_Size, filename.c_str());
    }

    void Font::setScale(float x, float y)
    {
        m_Scale = maths::vec2(x, y);
    }

        maths::vec2 Font::GetOffsets(const std::string& text) const
        {
            using namespace ftgl;

            if (text.empty())
                return maths::vec2(0.0f, 0.0f);

            texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[0]);
            //SP_ASSERT(glyph);

            float yo = 0.0f;
            const maths::vec2& scale = m_Scale;
            for (int i = 0; i < text.size(); i++)
            {
                texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
                //SP_ASSERT(glyph);
                float height = glyph->height / scale.y;
                float offset = glyph->offset_y / scale.y - height;
                if (offset < yo)
                    yo = offset;
            }

            return maths::vec2(glyph->offset_x / scale.x, yo);
        }

        float Font::GetWidth(const std::string& text) const
        {
            using namespace ftgl;

            float width = 0.0f;
            const maths::vec2& scale = m_Scale;
            for (int i = 0; i < text.size(); i++)
            {
                texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
                //SP_ASSERT(glyph);
                if (i > 0)
                {
                    float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
                    width += kerning / scale.x;
                }
                width += glyph->advance_x / scale.x;
            }
            return width;
        }

        float Font::GetHeight(const std::string& text) const
        {
            using namespace ftgl;

            const maths::vec2& scale = m_Scale;
            float min = 0.0f;
            float max = 0.0f;
            for (int i = 0; i < text.size(); i++)
            {
                texture_glyph_t* glyph = texture_font_get_glyph(m_FTFont, text[i]);
                //SP_ASSERT(glyph);
                float height = glyph->height / scale.y;
                float offset = glyph->offset_y / scale.y - height;
                if (offset < min)
                    min = offset;
                if (height > max)
                    max = height;
            }
            return abs(min) + abs(max);
        }

        maths::vec2 Font::GetSize(const std::string& text) const
        {
            return maths::vec2(GetWidth(text), GetHeight(text));
        }

        maths::Rectangle Font::GetBounds(const std::string& text) const
        {
            return maths::Rectangle(0.0f, 0.0f, GetWidth(text), GetHeight(text));
        }

} }