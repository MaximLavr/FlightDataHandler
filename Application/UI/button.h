#include "../../SparkyEngine/src/engine.h"

using namespace sparky;
using namespace graphics;

class Button :  public Group {

public:
    Button(float x, float y, float width, float height, unsigned int color, Label* text, Window* window);

    vec2 getPosition() const { return m_Position; }
    vec2 getSize() const { return m_Size; }
    unsigned int getColor() { return m_Color; }
    void setColorHover(vec4 color);
    void setColorPressed(vec4 color);
    void submit(Renderer2D* renderer) const;

    bool isClicked = false;
    ~Button();

    bool isPresed() { return m_IsPressed; }

private:
    Window* m_Window;
    vec2 m_Position;
    vec2 m_Size;
    unsigned int m_Color;
    vec4 m_ColorHover;
    vec4 m_ColorPressed;

    std::vector<Renderable2D*> m_Renderables;

    Shader* m_Shader;

    Renderable2D* m_HoverSprite;
    Renderable2D* m_PressedSprite;

    bool m_IsHovered = false;
    bool m_IsPressed = false;

    bool isHovered() const;
    bool isPressed() const;
};
