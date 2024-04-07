#pragma once
#include "../../SparkyEngine/src/engine.h"

using namespace sparky;
using namespace graphics;

class Button : public Group {

public:
    Button(float x, float y, float width, float height, unsigned int color, Renderable2D* textOrSprite, Window* window,
           vec4 colorHover = vec4(0, 0, 0, 0.15f), vec4 colorPressed = vec4(0, 0, 0, 0.5f));

    vec2 getPosition() const { return m_Position; }
    vec2 getSize() const { return m_Size; }
    unsigned int getColor() { return m_Color; }
    void setColorHover(vec4 color);
    void setColorPressed(vec4 color);
    void submit(Renderer2D* renderer) const;

    bool isClickedVar = false;
    ~Button() {
        delete m_Shader;
        delete m_HoverSprite;
        delete m_PressedSprite;
        for (auto& i : m_Renderables)
            delete i;
    };

    bool isPresed() { return m_IsPressed; }
     bool do1;
     bool prev_do1;
    bool m_IsHovered = false;
    bool isHovered() const;
    bool isPressed() const;
    bool isReleasedAfterPress() const;

    vec2 m_Position;
private:
    Window* m_Window;
    vec2 m_Size;
    unsigned int m_Color;
    vec4 m_ColorHover;
    vec4 m_ColorPressed;

    std::vector<Renderable2D*> m_Renderables;

    Shader* m_Shader;

    Renderable2D* m_HoverSprite;
    Renderable2D* m_PressedSprite;

    bool m_IsPressed = false;
    bool m_IsClicked = false;
    bool isClicked() const;

    bool canDo;

    bool flag;

    bool press;
    bool release;

    void DeleteRenderable(Renderable2D* renderable) const;
};
