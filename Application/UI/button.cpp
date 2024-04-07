#include "button.h"

using namespace sparky;
using namespace graphics;

Button::Button(float x, float y, float width, float height, unsigned int color, Label *text, Window* window,
    vec4 colorHover, vec4 colorPressed)
        : Group(mat4().translation(vec3(0, 0, 0)))
{
    m_Window = window;
    m_Position = vec2(x, y);
    m_Size = vec2(width, height);
    m_Color = color;


    //m_ColorHover = vec4(0, 0, 0, 0.15f);
    //m_ColorPressed = vec4(0, 0, 0, 0.5f);
    m_ColorHover = colorHover;
    m_ColorPressed = colorPressed;

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

/*void Button::DeleteRenderable(Renderable2D* renderable) const {
    auto it = std::find(m_Renderables.begin(), m_Renderables.end(), renderable);
    if(it != m_Renderables.end())
        m_Renderables.erase(it);
    //renderable->DeleteTexture();
    if ( m_Renderables.back()->m_Texture != nullptr)
        m_Renderables.back()->m_Texture->deleteTexture();
    delete renderable;
}*/

bool Button::isHovered() const {
#ifdef __APPLE__
    float x = m_Window->getMousePosition().x / (m_Window->getWidth() / m_Window->cameraRight / 2.0f) - m_Window->cameraRight;
    float y = -(m_Window->getMousePosition().y / (m_Window->getHeight() / m_Window->cameraUp / 2.0f) - m_Window->cameraUp);
#else
    float x = (m_Window->getMousePosition().x - m_Window->getWidth() / 2.0f) / 30.0f;
    float y = -(m_Window->getMousePosition().y - m_Window->getHeight() / 2.0f) / 30.0f;
#endif
    if ((x >= this->getPosition().x) && (x <= this->getPosition().x + this->getSize().x) &&
        (y >= this->getPosition().y) && (y <= this->getPosition().y + this->getSize().y)) {
        return true;
    }
    return false;
}

bool Button::isPressed() const {
    if (m_Window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
        //const_cast<bool&>(press) = true;
       return true;
    }
    return false;
}

bool Button::isClicked() const {
    if (m_Window->isMouseButtonClicked(GLFW_MOUSE_BUTTON_1)) {
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

#if 0
void Button::submit(Renderer2D* renderer) const
{
    if (this->isHovered() && !m_IsHovered)
    {
        const_cast<std::vector<Renderable2D *> &>(m_Renderables).push_back(m_HoverSprite);
        const_cast<bool&>(m_IsHovered) = true;
    }
    if (this->isHovered() && m_IsHovered)
    {
        if (this->isClicked())
        {
            std::cout << "CLICKED!" << std::endl;
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).push_back(m_PressedSprite);
        }
    }
    if (!this->isHovered() && m_IsHovered)
    {
        const_cast<std::vector<Renderable2D *> &>(m_Renderables).pop_back();
        const_cast<bool&>(m_IsHovered) = false;
    }

    renderer->push(m_TransformationMatrix);

    for (const Renderable2D* renderable : m_Renderables)
    {
        renderable->submit(renderer);
    }

    renderer->pop();
}
#endif


#if 1
void Button::submit(Renderer2D* renderer) const
{
#if 1
    if (this->isHovered()) {
        if (this->isPressed() && !m_IsPressed) {
            if (!isClickedVar)
                const_cast<bool&>(m_IsPressed) = true;
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).push_back(m_PressedSprite);
        }
        if (!this->isPressed() && m_IsPressed) {
            //m_Renderables.back()->DeleteTexture();
            if (m_Renderables.back()->m_Texture!= nullptr)
                m_Renderables.back()->m_Texture->deleteTexture();
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).pop_back();
            const_cast<bool&>(isClickedVar) = true;
        }
        else {
            const_cast<bool &>(isClickedVar) = false;
        }

        if (!m_IsHovered)
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).push_back(m_HoverSprite);

        const_cast<bool&>(m_IsHovered) = true;
    }
    if (!this->isHovered() && m_IsHovered) {
        //m_Renderables.back()->DeleteTexture();
        if (m_Renderables.back()->m_Texture!= nullptr)
            m_Renderables.back()->m_Texture->deleteTexture();
        const_cast<std::vector<Renderable2D*>&>(m_Renderables).pop_back();
        const_cast<bool&>(m_IsHovered) = false;
    }

    if (!this->isHovered() && m_IsPressed) {
        //m_Renderables.back()->DeleteTexture();
        if (m_Renderables.back()->m_Texture!= nullptr)
            m_Renderables.back()->m_Texture->deleteTexture();
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
    if (!this->isHovered() && !m_IsHovered) {
        if (m_Renderables.size() > 2) {
            if (m_Renderables.back()->m_Texture != nullptr)
                m_Renderables.back()->m_Texture->deleteTexture();
            const_cast<std::vector<Renderable2D *> &>(m_Renderables).pop_back();
        }
        const_cast<bool&>(m_IsHovered) = false;
    }

    renderer->pop();
#endif
}
#endif

