#include "label.h"

namespace sparky { namespace graphics {

    Label::Label(std::string text, float x, float y, unsigned int color)
        : Renderable2D(), text(text), position(m_Position), font(FontManager::get("SourceSansPro"))
    {
        m_Position = maths::vec3(x, y, 0.0f);
        m_Color = color;
        Text = std::move(text);
    }


    Label::Label(std::string text, float x, float y, Font* font, unsigned int color)
        : Renderable2D(), text(text), position(m_Position), font(font)
    {
        m_Position = maths::vec3(x, y, 0.0f);
        m_Color = color;
        Text = std::move(text);
    }

    Label::Label(std::string text, float x, float y, const std::string& font, unsigned int color)
            : Renderable2D(), text(text), position(m_Position), font(FontManager::get(font))
    {
        m_Position = maths::vec3(x, y, 0.0f);
        m_Color = color;
        Text = std::move(text);

        validateFont(font);
    }

    Label::Label(std::string text, float x, float y, const std::string& font, unsigned int size, unsigned int color)
            : Renderable2D(), text(text), position(m_Position), font(FontManager::get(font, size))
    {
        m_Position = maths::vec3(x, y, 0.0f);
        m_Color = color;
        Text = std::move(text);

        validateFont(font, size);
    }

    void Label::validateFont(const std::string& name, int size)
    {
        if (font != nullptr)
            return;

        std::cout << "NULL FONT! Font = " << name;
        if (size > 0)
            std::cout << ", Size = "<< size;
        std::cout << std::endl;

        font = FontManager::get("SourceSansPro");
    }

    void Label::submit(Renderer2D* renderer) const
    {
        renderer->drawString(text, m_Position, *font, m_Color);
    }

} }