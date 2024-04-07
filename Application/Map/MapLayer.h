#pragma once

#include "../../SparkyEngine/src/graphics/renderer2d.h"
#include "../../SparkyEngine/src/graphics/renderable2d.h"
#include "../../SparkyEngine/src/utils/DynamicArray.h"

namespace sparky { namespace graphics {

        class MapLayer
        {
        protected:
            Renderer2D* m_Renderer;
            maths::mat4 m_ProjectionMatrix;
            bool m_IsTransparent;
            int counter = 0;
        public:
            DynamicArray* m_Renderables;
            /*virtual*/ ~MapLayer() {
                delete m_Renderables;
                delete m_Shader;
                delete m_Renderer;
            };
            virtual void add(Renderable2D* renderable);
            //virtual void remove(Renderable2D* renderable);
            virtual void remove(Renderable2D* renderable);
            virtual void render();

            void remove_all();

            void renderPart(int indexBegin, int indexEnd);

            Shader* m_Shader;

            MapLayer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix, bool isTransparent);

            //inline const std::vector<Renderable2D*>& getRenderables() const { return m_Renderables; }

            int indexCounter = 0;

            void swapRenderables2D(Renderable2D& a, Renderable2D& b);
        };

    } }