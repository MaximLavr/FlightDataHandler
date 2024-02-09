#pragma once
#include "../../SparkyEngine/src/engine.h"

class StringDrawer : public Group
{
private:
    std::string m_String;
    vec2 m_Position;
    vec2 m_Size;
    unsigned int m_Color;
    Font* m_Font;
    float m_CurrentPositionX = 0;
    const float m_PaddingLetter = 0.0f;
    const float m_BarPadding = 1.0f;
    Sprite* bar;
    Window* Window;
public:
    StringDrawer(float x, float y, Font* font, unsigned int color, sparky::graphics::Window* window);
    vec2 getLength() { return m_Size.y; }
    float getCurrentPositionX() { return m_CurrentPositionX; }
    void addInput(char letter);
    bool isHovered() const;
};