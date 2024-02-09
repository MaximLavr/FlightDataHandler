#pragma once
#include "../renderable2d.h"
#include "../../utils/DynamicArray.h"

namespace sparky { namespace graphics {

    class Group : public Renderable2D
    {
    private:
    public:
        //std::vector<Renderable2D*> m_Renderables;
        DynamicArray* m_Renderables;
        maths::mat4 m_TransformationMatrix;
        Group(const maths::mat4& transform);
        ~Group();
        void add(Renderable2D* renderable);
        void remove();
        void remove(Renderable2D* renderable);
        void remove_all();
        void submit(Renderer2D* renderer) const override;

        maths::mat4& getTransformRef() { return m_TransformationMatrix; }
    };

} }