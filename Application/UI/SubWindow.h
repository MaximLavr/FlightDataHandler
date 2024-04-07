#pragma once
#include "../../SparkyEngine/src/engine.h"
#include "../Map/MapLayer.h"

class SubWindow : public MapLayer {
public:
    SubWindow(float x, float y, float width, float height, const char* title, Shader* shaderScroll, Shader* shaderSubWindow,
              mat4 ortho, Window* window,
              unsigned int colorHeader = transform_color(0.28647, 0.18647, 0.19824, 1),
              unsigned int colorBox =  transform_color(0.12647, 0.12647, 0.13824, 1),
              unsigned int colorText = transform_color(1, 1, 1, 1));
    const char* GetTitle() { return m_Title; }
    void ScrollUp(float ts);
    void ScrollDown(float ts);
    void ScrollLeft(float ts);
    void ScrollRight(float ts);
    void Render();
    void AddDataText(Label* renderable2D);
    void AddDataImage(Sprite* renderable2D);
    void DrawBorders();
    void ClearData();
    void RemoveData();

    float m_RenderableMinPositionY = MAXFLOAT;
    float m_RenderableMaxPositionY = 0;
    float m_RenderableMinPositionX = MAXFLOAT;
    float m_RenderableMaxPositionX = 0;
    float m_Speed = 0.4f;
    //const float m_SlowSpeed = 1.0f;
    const float m_StartSpeed = m_Speed;
    vec2 m_Position;
    vec2 m_Size;
    bool isHovered() const;
    float maxUpPosition = 0;
    float minLeftPosition = 0;
    float maxRightPosition = 0;
    Window* m_Window;

    float maxY = 0;
    float minY = 0;

    float minX = 0;
    float maxX = 0;

    /*old version*/
    float max = 0;
    float min = 0;
    /*old version*/

    float m_PaddingUp = 0.0f;
    float m_PaddingDown = 0.0f;
    float m_PaddingRight = 0.0f;
    float m_PaddingLeft = 0.0f;

    float m_StringInterval = 0.0f;

    const float m_HeaderHeight = 1.7f;//1.7f;

    float m_PaddingParagraph = 0.0;
    float m_TextStringHeight;

    float stringWidth;
    float maxStringWidth = 0;
    MapLayer* m_DataLayer;
    bool needToRender = true;

    float imageWidth;
    float maxImageWidth = 0;
    float currentMaxY;

private:
    const char* m_Title;

    Label* m_Label;

    Group* m_Body;
    Shader* m_ShaderScroll;
    Shader* m_ShaderSubWindow;

    unsigned int m_ColorBox;
    unsigned int m_ColorText;
    unsigned int m_ColorHeader;

    Sprite* m_SliderVertical;
    Sprite* m_SliderHorizontal;
    float percentAdded = 0;
    float maxHeight = 0;
    float deltaPosition = 0;

    bool flag = true;
};

