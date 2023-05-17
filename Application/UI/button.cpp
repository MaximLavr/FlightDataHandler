#include "button.h"

using namespace sparky;
using namespace graphics;

Button::Button(float x, float y, float width, float height, unsigned int color, Label *text, Window* window)
        : Group(mat4().translation(vec3(0, 0, 0)))
{
    m_Window = window;
    m_Position = vec2(x, y);
    m_Size = vec2(width, height);
    m_Color = color;

    m_ColorHover = vec4(0, 0, 0, 0.15f);
    m_ColorPressed = vec4(0, 0, 0, 0.5f);

    text->position.x += x;
    text->position.y += y;

    auto* sprite = new Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), color);
    m_Renderables.push_back(sprite);
    m_Renderables.push_back(text);

    m_HoverSprite = new Sprite(x, y,
                             width, height,
                             transform_color(m_ColorHover.x, m_ColorHover.y, m_ColorHover.z, m_ColorHover.w));

    m_PressedSprite = new Sprite(x, y,
                               width, height,
                               transform_color(m_ColorPressed.x, m_ColorPressed.y, m_ColorPressed.z, m_ColorPressed.w));

    this->add(sprite);
    this->add(text);
}

bool Button::isHovered() const {
    float x = (m_Window->getMousePosition().x - m_Window->getWidth() / 4.0f) / 30.0f;
    float y = -(m_Window->getMousePosition().y - m_Window->getHeight() / 4.0f) / 30.0f;

    if ((x >= this->getPosition().x) && (x <= this->getPosition().x + this->getSize().x) &&
        (y >= this->getPosition().y) && (y <= this->getPosition().y + this->getSize().y)) {
        return true;
    }
    return false;
}

bool Button::isPressed() const {
    if (m_Window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
        return true;
    }
    return false;
}

void Button::setColorHover(vec4 color)
{
    m_ColorHover = color;
    m_HoverSprite->setColor(m_ColorHover);
}

void Button::setColorPressed(vec4 color)
{
    m_ColorPressed = color;
    m_PressedSprite->setColor(m_ColorPressed);
}

void Button::submit(Renderer2D* renderer) const
{
    if (this->isHovered()) {
        if (this->isPressed() && !m_IsPressed) {
            if (!isClicked)
                const_cast<bool&>(isClicked) = true;
            const_cast<bool&>(m_IsPressed) = true;
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).push_back(m_PressedSprite);
        }
        if (!this->isPressed() && m_IsPressed) {
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).pop_back();
            const_cast<bool&>(isClicked) = true;
        }
        else
            const_cast<bool&>(isClicked) = false;

        if (!m_IsHovered)
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).push_back(m_HoverSprite);

        const_cast<bool&>(m_IsHovered) = true;
    }
    if (!this->isHovered() && m_IsHovered) {
        const_cast<std::vector<Renderable2D*>&>(m_Renderables).pop_back();
        const_cast<bool&>(m_IsHovered) = false;
    }

    if (!this->isHovered() && m_IsPressed) {
        const_cast<std::vector<Renderable2D*>&>(m_Renderables).pop_back();
        const_cast<bool&>(m_IsHovered) = false;
        const_cast<bool&>(m_IsPressed) = false;
    }

    if (!this->isPressed() && m_IsPressed) {
        const_cast<bool&>(m_IsPressed) = false;
    }

    renderer->push(m_TransformationMatrix);

    for (const Renderable2D* renderable : m_Renderables)
    {
        renderable->submit(renderer);
    }

    renderer->pop();
}

Button::~Button()
{
    for (auto r : m_Renderables)
        delete r;
}

