#define LIGHT_CASTING 0
#include "layer.h"
#include "../../utils/VectorRemoval.h"

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
                0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
                10, 11, 12, 13, 14, 15
        };

        m_Shader->setUniform1iv("textures", texIDs, 16);

        //m_Shader->disable();
    }

    Layer::~Layer()
    {
#if 1
        if (m_Shader != nullptr)
            delete m_Shader;
        if (m_Renderer != nullptr)
            delete m_Renderer;

        //delete m_Shader;
        //delete m_Renderer;
        for (int i = 0; i < m_Renderables.size(); i++)
        {
            delete m_Renderables[i];
        }
        m_Renderables.clear();
#endif
    }

    void Layer::add(Renderable2D* renderable)
    {
        m_Renderables.push_back(renderable);
    }

    void Layer::remove(Renderable2D* renderable)
    {
        //m_Renderables.erase(std::remove(m_Renderables.begin(),
          //                              m_Renderables.end(), renderable), m_Renderables.end());


        auto it = std::find(m_Renderables.begin(), m_Renderables.end(), renderable);
        if(it != m_Renderables.end())
            m_Renderables.erase(it);
        if (renderable->m_Texture != nullptr)
            renderable->m_Texture->deleteTexture();

        delete renderable;
#if 0
        if (renderable != nullptr)
        {
            swapRenderables2D(*m_Renderables.back(), *renderable);
            m_Renderables.pop_back();
            //delete renderable;
            //renderable = nullptr;
            //*renderable = NULL;
        }
#endif
        //m_Renderables.erase(std::remove(m_Renderables.begin(), m_Renderables.end(), renderable), m_Renderables.end());

        //sparky::VectorRemoval::remove_element(m_Renderables, index);
    }

    void Layer::renderPart(int indexBegin, int indexEnd)
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

        for (int i = indexBegin; i < indexEnd; i++)
        {
            if (m_Renderables[i] != nullptr)
                m_Renderables[i]->submit(m_Renderer);
        }

        m_Renderer->end();
        m_Renderer->flush();
    }

    void Layer::remove_all()
    {
        for (auto& i : m_Renderables)
        {
            if (i->m_Texture != nullptr)
                i->m_Texture->deleteTexture();
            delete i;
        }
        m_Renderables.clear();
        /*if (m_Renderables.size() > 0)
        {
            for (int i = 0; i < m_Renderables.size(); i++)
            {
                if (m_Renderables[i] != nullptr)
                    this->remove(m_Renderables[i]);
            }
        }*/
        indexCounter = 0;
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
    
        for (const Renderable2D* renderable : m_Renderables) {
            if (renderable != nullptr)
                renderable->submit(m_Renderer);
        }

#if LIGHT_CASTING
        m_Renderer->DrawLine(maths::vec2(0, 0), maths::vec2(0, 1), 0xffffffff, 0.006);
        m_Renderer->DrawLine(maths::vec2(0, 1), maths::vec2(1, 1), 0xffffffff, 0.006);
        m_Renderer->DrawLine(maths::vec2(1, 1), maths::vec2(1, 0), 0xffffffff, 0.006);
        m_Renderer->DrawLine(maths::vec2(1, 0), maths::vec2(0, 0), 0xffffffff, 0.006);
#endif

        m_Renderer->end(); 
        m_Renderer->flush();
        //m_Shader->disable();
    }

    void Layer::swapRenderables2D(Renderable2D& a, Renderable2D& b) {
        auto temp = a;
        a = b;
        b = temp;
    }

} }