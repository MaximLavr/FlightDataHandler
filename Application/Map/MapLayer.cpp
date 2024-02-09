#define LIGHT_CASTING 0
#include "MapLayer.h"
#include "../../SparkyEngine/src/utils/VectorRemoval.h"

namespace sparky { namespace graphics {

        MapLayer::MapLayer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix, bool isTransparent)
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

            m_Renderables = new DynamicArray();

            //m_Shader->disable();
        }

        void MapLayer::add(Renderable2D* renderable)
        {
            m_Renderables->add(renderable);
        }

        void MapLayer::remove(Renderable2D* renderable)
        {
            //m_Renderables.erase(std::remove(m_Renderables.begin(),
            //                              m_Renderables.end(), renderable), m_Renderables.end());


            /*auto it = std::find(m_Renderables.begin(), m_Renderables.end(), renderable);
            if(it != m_Renderables.end())
                m_Renderables.erase(it);*/
            //if (!m_Renderables.empty())
            //  m_Renderables.pop_back();

            //if (m_Renderables[renderable->index] != nullptr)
              //  std::cout << "del!\n";
                //delete m_Renderables[renderable->index];




            //sparky::VectorRemoval::remove_element(m_Renderables, index);

            m_Renderables->remove(renderable);
            if (renderable->m_Texture != nullptr)
                renderable->m_Texture->deleteTexture();

            //delete renderable;
        }

        void MapLayer::renderPart(int indexBegin, int indexEnd)
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
                m_Renderables->getArr()[i]->submit(m_Renderer);
            }

            m_Renderer->end();
            m_Renderer->flush();
        }

        void MapLayer::remove_all()
        {
#if 0
            /*for (int i = 0; i < m_Renderables->getSize(); i++)
            {
                this->remove(m_Renderables->getArr()[i]);
                //m_Renderables->getArr()[i]->DeleteTexture();
                //delete m_Renderables->getArr()[i];
            }*/
            m_Renderables->getArr().clear();
            indexCounter = 0;
#endif
            for (auto& i : m_Renderables->getArr())
            {
                m_Renderables->remove(i);
                if (i->m_Texture != nullptr)
                    i->m_Texture->deleteTexture();
                delete i;
            }
            m_Renderables->getArr().clear();
            indexCounter = 0;
        }

        void MapLayer::render()
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

            for (const Renderable2D* renderable : m_Renderables->getArr()) {
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

        void MapLayer::swapRenderables2D(Renderable2D &a, Renderable2D &b) {
            auto temp = a;
            a = b;
            b = temp;
        }

    } }