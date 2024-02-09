//
// Created by Максим Лавриненко on 18.01.2024.
//

#include "StringDrawer.h"

StringDrawer::StringDrawer(float x, float y, Font *font, unsigned int color, sparky::graphics::Window* window)
        : Group(mat4().translation(vec3(0, 0, 0)))
{
    m_Position = vec2(x, y);
    m_Color = color;
    m_Font = font;
    Window = window;

    float height = FontManager::get("Lusia_UI")->GetHeight("W");

    bar = new Sprite(x - m_BarPadding, y - m_BarPadding / 2.0f, 10.0f, height + m_BarPadding, transform_color(0.1, 0.1, 0.1, 1));
    this->add(bar);
}

void StringDrawer::addInput(char letter) {
    m_String.push_back(letter);

    std::string str(1, letter);

    Label* l = new Label(str, m_Position.x + m_CurrentPositionX, m_Position.y, m_Font, m_Color);
    m_CurrentPositionX += m_Font->GetWidth(str);
    m_CurrentPositionX += m_PaddingLetter;

    this->add(l);
    //this->add(new Sprite(0, 0, 10, 10, transform_color(1, 0, 1, 1)));
}

bool StringDrawer::isHovered() const {
#ifdef __APPLE__
    float x = Window->getMousePosition().x / (Window->getWidth() / Window->cameraRight / 2.0f) - Window->cameraRight;
    float y = -(Window->getMousePosition().y / (Window->getHeight() / Window->cameraUp / 2.0f) - Window->cameraUp);
#else
    float x = (m_Window->getMousePosition().x - m_Window->getWidth() / 2.0f) / 30.0f;
    float y = -(m_Window->getMousePosition().y - m_Window->getHeight() / 2.0f) / 30.0f;
#endif
    if ((x >= bar->getPosition().x) && (x <= bar->getPosition().x + bar->getSize().x) &&
        (y >= bar->getPosition().y) && (y <= bar->getPosition().y + bar->getSize().y)) {
        return true;
    }
    return false;
}


