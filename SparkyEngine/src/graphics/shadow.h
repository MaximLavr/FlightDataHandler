#pragma once
#include "../maths/maths.h"
#include "../graphics/renderable2d.h"
#include "../graphics/batchrenderer2d.h"
#include <cmath>
#include <GL/glew.h>

namespace sparky { namespace graphics {

    class Shadow : public Renderable2D
    {
    private:
        maths::vec2 m_LightPosition;
        maths::vec4 m_WallPosition;
        float m_Radius;
        std::vector<float> m_Vertices;
    protected:
        std::vector<float> getVertices() { return m_Vertices; }
    public:
        Shadow(maths::vec2 light_position, maths::vec4 wall_position, float lightRadius, Texture* texture);
        Shadow(maths::vec2 light_position, maths::vec4 wall_position, float lightRadius, unsigned int color);
        //add constructors without lightRadius and color
        void submit(Renderer2D* renderer) const override;
        void setLightPosition(maths::vec2 light_position) { m_LightPosition = light_position; }
        void setWallPosition(maths::vec4 wall_position) { m_WallPosition = wall_position; }
        void setRadius(float radius) { m_Radius = radius; }
        void setColor(unsigned int color) { m_Color = color; }
    };

} }