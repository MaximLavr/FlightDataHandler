#define LIGHT_CASTING 0
#include "layer.h"

namespace sparky { namespace graphics {

    Layer::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix, bool isTransparent)
    {
        m_ProjectionMatrix = projectionMatrix;
        m_Renderer = renderer;
        m_Shader = shader;
        m_IsTransparent = isTransparent;
        
        m_Shader->enable();
        m_Shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);

        GLint texIDs[] = {
                0, 1, 2, 3, 4, 5, 6, 7, 8,9,
                10, 11, 12, 13, 14, 15, 16, 17, 18,19,
                20, 21, 22, 23, 24, 25, 26, 27, 28,29,
                30, 31
        };

        m_Shader->setUniform1iv("textures", texIDs,32);

        m_Shader->disable();
    }

    Layer::~Layer()
    {
        delete m_Shader;
        delete m_Renderer;
        for (int i = 0; i < m_Renderables.size(); i++)
        {
            delete m_Renderables[i];
        }
    }

    void Layer::add(Renderable2D* renderable)
    {
        m_Renderables.push_back(renderable);
    }

    void Layer::remove(Renderable2D* renderable)
    {
        m_Renderables.erase(std::remove(m_Renderables.begin(),
                                        m_Renderables.end(), renderable), m_Renderables.end());
    }

    void Layer::render()
    {
        if (m_IsTransparent)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            //glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
        }
        else
            glDisable(GL_BLEND);

        m_Shader->enable();

        m_Renderer->begin();
    
        for (const Renderable2D* renderable : m_Renderables)
            renderable->submit(m_Renderer);

#if LIGHT_CASTING
        m_Renderer->DrawLine(maths::vec2(0, 0), maths::vec2(0, 1), 0xffffffff, 0.006);
        m_Renderer->DrawLine(maths::vec2(0, 1), maths::vec2(1, 1), 0xffffffff, 0.006);
        m_Renderer->DrawLine(maths::vec2(1, 1), maths::vec2(1, 0), 0xffffffff, 0.006);
        m_Renderer->DrawLine(maths::vec2(1, 0), maths::vec2(0, 0), 0xffffffff, 0.006);
#endif

        m_Renderer->end(); 
        m_Renderer->flush();
        m_Shader->disable();
    }

} }