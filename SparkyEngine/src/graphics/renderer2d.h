#pragma once

#include <vector>
#include <GL/glew.h>
#include "../maths/maths.h"
#include "font.h"
#include "shader.h"

namespace sparky { namespace graphics {

    class Renderable2D;

    class Renderer2D
    {
    protected:
        std::vector<maths::mat4> m_TransformationStack;
        maths::mat4* m_TransformationBack;
    protected:
        Renderer2D()
        {
            m_TransformationStack.push_back(maths::mat4::identity());
            m_TransformationBack = &m_TransformationStack.back();
        }
    public:
        virtual ~Renderer2D() {}
        //virtual void DrawRect(float x, float y, float width, float height, unsigned int color = 0xffffffff) { }
        //virtual void DrawRect(const maths::vec2& position, const maths::vec2& size, unsigned int color = 0xffffffff) { }
        //virtual void DrawRect(const maths::Rectangle& rectangle, unsigned int color = 0xffffffff) { }
        void push(const maths::mat4& matrix, bool override = false)
        {
            if (override)
            {
                m_TransformationStack.push_back(matrix);    
            }
            else 
            {
                m_TransformationStack.push_back(m_TransformationStack.back() * matrix);
            }

            m_TransformationBack = &m_TransformationStack.back();
        }
        void pop()
        {
            if (m_TransformationStack.size() > 1)
            {
                m_TransformationStack.pop_back();
            }

            m_TransformationBack = &m_TransformationStack.back();
        }

        virtual void begin() {}
        virtual void submit(const Renderable2D* renderable) = 0;
        virtual void drawString(const std::string& text, const maths::vec3& position,
                                const Font& font, unsigned int color) {}
        virtual void end() {}
        virtual void flush() = 0;

        virtual void DrawRect(float x, float y, float width, float height, unsigned int color = 0xffffffff) { }
        virtual void DrawRect(const maths::vec2& position, const maths::vec2& size, unsigned int color = 0xffffffff) { }
        virtual void DrawRect(const maths::Rectangle& rectangle, unsigned int color = 0xffffffff) { }
        virtual void drawFigure(std::vector<float> vertices, unsigned int color) { }
        virtual void drawShadow(Renderable2D* renderable) { }

        virtual void DrawLine(const maths::vec2& start, const maths::vec2& end, unsigned int color = 0xffffffff, float thickness = 0.02f) { }

    };
} }
