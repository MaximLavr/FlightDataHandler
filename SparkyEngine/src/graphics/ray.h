#pragma once
#include "../maths/maths.h"
#include "renderable2d.h"

using namespace sparky::maths;

namespace sparky { namespace graphics {

    class Ray : public Renderable2D
    {
    public:
        Ray(vec2 position, float angle);
        bool isHitWall(vec4 wall);
        vec2 getWallPoint(vec4 wall);

        vec2& getPosition() { return m_Position; }
        vec2& getDirection() { return m_Direction; }

        void setPosition(vec2 position) { m_Position = position; }
        void setDirection(vec2 direction) { m_Direction = direction; }
    private:
        float m_T;
        vec2 m_Position;
        vec2 m_Direction;
        void submit(Renderer2D* renderer) const override;
    };

} }