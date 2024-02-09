#pragma once

#include "../renderer2d.h"
#include "../renderable2d.h"

namespace sparky { namespace graphics {

    class Layer
    {
    protected:
        maths::mat4 m_ProjectionMatrix;
        bool m_IsTransparent;
    public:
        Renderer2D* m_Renderer;
        std::vector<Renderable2D*> m_Renderables;
        /*virtual*/ ~Layer();
        virtual void add(Renderable2D* renderable);
        //virtual void remove(Renderable2D* renderable);
        virtual void remove(Renderable2D* renderable);
        virtual void render();

        void remove_all();

        void renderPart(int indexBegin, int indexEnd);

        Shader* m_Shader;

        Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix, bool isTransparent);

        inline const std::vector<Renderable2D*>& getRenderables() const { return m_Renderables; }

        int indexCounter = 0;

        void swapRenderables2D(Renderable2D& a, Renderable2D& b);
    };

} }