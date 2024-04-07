#include "group.h"

namespace sparky { namespace graphics {

    Group::Group(const maths::mat4& transform)
        : m_TransformationMatrix(transform)
    {
        m_Renderables = new DynamicArray();
    }

    Group::~Group()
    {
        for (int i = 0; i < m_Renderables->getSize(); i++)
        {
            delete m_Renderables->getArr()[i];
        }
        delete m_Renderables;
    }

    void Group::add(Renderable2D* renderable)
    {
        m_Renderables->add(renderable);
    }

    void Group::remove(Renderable2D* renderable)
    {
        /*auto it = std::find(m_Renderables.begin(), m_Renderables.end(), renderable);
        if(it != m_Renderables.end())
            m_Renderables.erase(it);*/
        //m_Renderables->remove(renderable);
        m_Renderables->remove(renderable);
        //renderable->DeleteTexture();
        if (renderable->m_Texture != nullptr)
            renderable->m_Texture->deleteTexture();
        delete renderable;
    }

    void Group::remove_all()
    {
        /*for (auto& i : m_Renderables->getArr())
        {
            m_Renderables->remove(i);
            i->DeleteTexture();
            delete i;
        }
        m_Renderables->getArr().clear();*/
        for (auto& i : m_Renderables->getArr())
        {
            m_Renderables->remove(i);
            //i->DeleteTexture();
            if (i->m_Texture != nullptr)
                i->m_Texture->deleteTexture();
            delete i;
        }
        m_Renderables->getArr().clear();

    }

    void Group::submit(Renderer2D* renderer) const
    {
        renderer->push(m_TransformationMatrix);

        for (const Renderable2D* renderable : m_Renderables->getArr())
        {
            renderable->submit(renderer);
        }

        renderer->pop();
    }

} }