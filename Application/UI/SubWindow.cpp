#define SLOW_SPEED 0

#include "SubWindow.h"

SubWindow::SubWindow(float x, float y, float width, float height, const char *title, Shader* shaderScroll, Shader* shaderSubWindow,
                     mat4 ortho, Window* window,
                     unsigned int colorHeader, unsigned int colorBox, unsigned int colorText)
        : MapLayer(new BatchRenderer2D(window), shaderSubWindow, ortho, true)
{
    m_Window = window;
    m_Position = vec2(x, y);
    m_Size = vec2(width, height);
    m_Title = title;
    m_ShaderScroll = shaderScroll;
    m_ShaderSubWindow = shaderSubWindow;
    m_ColorHeader = colorHeader;
    m_ColorBox = colorBox;
    m_ColorText = colorText;

    m_DataLayer = new MapLayer(new BatchRenderer2D(window), m_ShaderScroll, ortho, true);

    m_ShaderScroll->setUniform1f("left_position", m_Position.x);
    m_ShaderScroll->setUniform1f("right_position", m_Position.x + m_Size.x);
    m_ShaderScroll->setUniform1f("up_position", m_Position.y + m_Size.y - m_HeaderHeight);
    m_ShaderScroll->setUniform1f("down_position",  m_Position.y);

    m_ShaderSubWindow->setUniform1f("left_position", m_Position.x);
    m_ShaderSubWindow->setUniform1f("right_position", m_Position.x + m_Size.x);
    m_ShaderSubWindow->setUniform1f("up_position", m_Position.y + m_Size.y);
    m_ShaderSubWindow->setUniform1f("down_position",  m_Position.y);

    Sprite* header = new Sprite(m_Position.x, m_Position.y + m_Size.y - m_HeaderHeight, m_Size.x, m_HeaderHeight, m_ColorHeader);
    this->add(header);

    Sprite* box = new Sprite(m_Position.x, m_Position.y, m_Size.x, m_Size.y - m_HeaderHeight, m_ColorBox);
    this->add(box);

    m_Label =  new Label(title, m_Position.x +  FontManager::get("Lusia_UI")->getScale().x / 70.0f, m_Position.y + m_Size.y - m_HeaderHeight + FontManager::get("Lusia_UI")->getScale().y / 100.0f, "Lusia_UI", m_ColorText);
    this->add(m_Label);

    m_Body = new Group(mat4::translation((maths::vec3(m_Position.x, m_Position.y, 0.0f))));
    m_DataLayer->add(m_Body);

    m_RenderableMaxPositionY = m_Size.y - m_HeaderHeight - m_PaddingUp;
    m_RenderableMinPositionX = m_Size.x + m_PaddingLeft;

    //m_Speed = (float)window->getWidth() / 2500.0f;
    //m_Speed /= (float)window->getWidth() / 2500.0f;

    //maxY = m_PaddingUp;
    //minY = m_PaddingDown;

    //minX = m_PaddingLeft;
    //maxX = m_PaddingRight;
}

//BUTTON: DOWN
void SubWindow::ScrollDown(float ts) {
    //if (m_RenderableMinPositionY < m_PaddingDown)
    //if (m_RenderableMinPositionY + (m_Speed * ts) < 0 + m_PaddingDown)
    {
#if SLOW_SPEED
        if (m_RenderableMinPositionY + m_Speed > 0) //- m_PaddingDown/* + m_PaddingDown*/)
            m_Speed = m_SlowSpeed;
        else
            m_Speed = m_StartSpeed;
#endif
        for (auto& r : m_Body->m_Renderables->getArr())
        {
            r->m_Position.y += (m_Speed * ts);
        }
        m_RenderableMinPositionY += (m_Speed * ts);
        m_RenderableMaxPositionY += (m_Speed * ts);
    }
}

void SubWindow::ScrollUp(float ts) {
    //if (m_RenderableMaxPositionY > m_Size.y - m_HeaderHeight/* - maxUpPosition*/)
    //if (m_RenderableMaxPositionY - (m_Speed * ts) > m_Size.y - m_HeaderHeight/* - maxUpPosition*/)
    {
#if 1
        /*if (m_RenderableMaxPositionY > m_Size.y - m_HeaderHeight &&
        m_RenderableMaxPositionY - m_Speed < m_Size.y - m_HeaderHeight - m_PaddingUp) {
            m_Speed = m_SlowSpeed;
            std::cout << "slow up\n";
        }
        else*/
        //m_Speed = m_StartSpeed;
#endif
        for (auto& r : m_Body->m_Renderables->getArr())
        {
            r->m_Position.y -= (m_Speed * ts);
        }
        m_RenderableMinPositionY -= (m_Speed * ts);
        m_RenderableMaxPositionY -= (m_Speed * ts);
    }
}

void SubWindow::ScrollLeft(float ts) {
    if (m_RenderableMinPositionX + (m_Speed) < m_PaddingLeft)
    //if (true)
    {
#if 0
        if (m_RenderableMinPositionX + m_Speed > m_PaddingLeft)
            m_Speed = m_SlowSpeed;
        else
            m_Speed = m_StartSpeed;
#endif
        for (auto& r : m_Body->m_Renderables->getArr())
        {
            r->m_Position.x += (m_Speed * ts );
        }
        m_RenderableMinPositionX += (m_Speed * ts );
        m_RenderableMaxPositionX += (m_Speed * ts);
    }
}

//BUTTON: UP

void SubWindow::ScrollRight(float ts) {
    //if (m_RenderableMaxPositionX > m_Size.x - maxRightPosition)
    if (m_RenderableMaxPositionX  > m_Size.x - m_PaddingRight)
    //if (true)
    {
#if SLOW_SPEED
        if (m_RenderableMaxPositionX - m_Speed < m_Size.x - m_PaddingRight)
            m_Speed = m_SlowSpeed;
#endif
        for (auto& r : m_Body->m_Renderables->getArr())
        {
            r->m_Position.x -= (m_Speed);
        }
        m_RenderableMinPositionX -= (m_Speed);
        m_RenderableMaxPositionX -= (m_Speed);
    }
}

void SubWindow::Render()
{
    this->render();
    if (needToRender)
        m_DataLayer->render();
}

void SubWindow::AddDataText(Label* renderable2D) {
    m_Body->add(renderable2D);
    auto absPos = vec2(renderable2D->m_Position.x + m_Position.x, renderable2D->m_Position.y + m_Position.y);
    //deltaPosition = 0;
    /*if (absPos.y + renderable2D->m_Size.y > m_Position.y + m_Size.y - m_HeaderHeight) {
        auto dif = absPos.y + renderable2D->m_Size.y - (m_Position.y + m_Size.y - m_HeaderHeight);
        if (dif > maxY)
            maxY = dif;
    }
    if (renderable2D->m_Position.y < 0) {
        if (renderable2D->m_Position.y < minY) {
            minY = renderable2D->m_Position.y;
        }
    }*/

    auto textWidth = renderable2D->font->GetWidth(renderable2D->text);
    if (renderable2D->m_Position.y + textWidth < m_Size.y &&
        renderable2D->m_Position.y + textWidth > maxUpPosition) {
        maxUpPosition = renderable2D->m_Position.y + textWidth;
    }
    if (renderable2D->m_Position.x < 0 &&
        renderable2D->m_Position.x < minLeftPosition) {
        minLeftPosition = renderable2D->m_Position.x;
    }


    //stringWidth = FontManager::get("Lusia_UI")->GetWidth(renderable2D->Text);
    if (textWidth > maxStringWidth)
        maxStringWidth = textWidth;
    //66.0

    if (renderable2D->m_Position.y < m_RenderableMinPositionY)
        m_RenderableMinPositionY = renderable2D->m_Position.y;
    if (renderable2D->m_Position.y + renderable2D->font->GetWidth(renderable2D->text) > m_RenderableMaxPositionY)
        m_RenderableMaxPositionY = renderable2D->m_Position.y + renderable2D->font->GetWidth(renderable2D->text);

    if (renderable2D->m_Position.x < m_RenderableMinPositionX)
        m_RenderableMinPositionX = renderable2D->m_Position.x;
    if (renderable2D->m_Position.x + renderable2D->font->GetWidth(renderable2D->text) > m_RenderableMaxPositionX)
        m_RenderableMaxPositionX = renderable2D->m_Position.x + renderable2D->font->GetWidth(renderable2D->text);
#if 0
    if (/*renderable2D->m_Position.x +*/ stringWidth > m_RenderableMaxPositionX) {
        m_RenderableMaxPositionX = /*renderable2D->m_Position.x +*/ stringWidth;
    }
#endif

    //std::cout << "m_RenderableMinPositionX = " << m_RenderableMinPositionX << std::endl;

    if (m_RenderableMaxPositionY != maxUpPosition)
        maxUpPosition = 0;

    if (m_RenderableMinPositionX != minLeftPosition)
        minLeftPosition = 0;

    /*if (m_Position.y + m_RenderableMaxPositionY <= m_Position.y + m_Size.y - m_HeaderHeight) {
        maxY += m_Size.y - m_HeaderHeight - m_RenderableMaxPositionY;
    }*/
}

void SubWindow::AddDataImage(Sprite* renderable2D) {
    m_Body->add(renderable2D);
    auto absPos = vec2(renderable2D->m_Position.x + m_Position.x, renderable2D->m_Position.y + m_Position.y);

    if (renderable2D->m_Position.y + renderable2D->m_Size.y < m_Size.y &&
        renderable2D->m_Position.y + renderable2D->m_Size.y > maxUpPosition) {
        maxUpPosition = renderable2D->m_Position.y + renderable2D->m_Size.y;
    }
    if (renderable2D->m_Position.x < 0 &&
        renderable2D->m_Position.x < minLeftPosition) {
        minLeftPosition = renderable2D->m_Position.x;
    }
    imageWidth = renderable2D->getSize().x;
    if (imageWidth > maxImageWidth)
        maxImageWidth = stringWidth;

    if (renderable2D->m_Position.y < m_RenderableMinPositionY)
        m_RenderableMinPositionY = renderable2D->m_Position.y;
    if (renderable2D->m_Position.y + renderable2D->m_Size.y > m_RenderableMaxPositionY)
        m_RenderableMaxPositionY = renderable2D->m_Position.y + renderable2D->m_Size.y;

    if (renderable2D->m_Position.x < m_RenderableMinPositionX)
        m_RenderableMinPositionX = renderable2D->m_Position.x;
    if (renderable2D->m_Position.x + stringWidth > m_RenderableMaxPositionX)
        m_RenderableMaxPositionX = renderable2D->m_Position.x + stringWidth;

    if (m_RenderableMaxPositionY != maxUpPosition)
        maxUpPosition = 0;

    if (m_RenderableMinPositionX != minLeftPosition)
        minLeftPosition = 0;
}


bool SubWindow::isHovered() const {
#ifdef __APPLE__
    float x = m_Window->getMousePosition().x / (m_Window->getWidth() / m_Window->cameraRight / 2.0f) - m_Window->cameraRight;
    float y = -(m_Window->getMousePosition().y / (m_Window->getHeight() / m_Window->cameraUp / 2.0f) - m_Window->cameraUp);
#else
    float x = (m_Window->getMousePosition().x - m_Window->getWidth() / 2.0f) / 30.0f;
    float y = -(m_Window->getMousePosition().y - m_Window->getHeight() / 2.0f) / 30.0f;
#endif
    if ((x >= this->m_Position.x) && (x <= this->m_Position.x + this->m_Size.x) &&
        (y >= this->m_Position.y) && (y <= this->m_Position.y + this->m_Size.y)) {
        return true;
    }
    return false;
}

void SubWindow::ClearData()
{
    if (m_Renderables->getSize() > 0)
    {
        /*for (int i = 0; i < m_Renderables->getSize(); i++)
        {
            m_Renderables->remove(m_Renderables->getArr()[i]);
            //if (m_Renderables[i] != nullptr)
              //  this->remove(m_Renderables[i]);
            //delete m_Renderables[i];
        }*/
        for (Renderable2D* renderable : m_Renderables->getArr())
        {
            m_Renderables->remove(renderable);
        }
    }
}

void SubWindow::RemoveData()
{
    if (m_DataLayer->m_Renderables->getSize() > 0)
    {
        /*for (int i = 0; i < m_Renderables->getSize(); i++)
        {
            m_Renderables->remove(m_Renderables->getArr()[i]);
            //if (m_Renderables[i] != nullptr)
              //  this->remove(m_Renderables[i]);
            //delete m_Renderables[i];
        }*/
        for (Renderable2D* renderable : m_DataLayer->m_Renderables->getArr())
        {
            m_DataLayer->m_Renderables->remove(renderable);
        }
    }
}
