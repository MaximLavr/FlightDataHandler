#include "shadow.h"

namespace sparky { namespace graphics {
        float distance(maths::vec2 vector)
        {
            return std::hypot(vector.x, vector.y);
        }

        maths::vec2 unitVector(maths::vec2 vector)
        {
            const float d = distance(vector);
            return maths::vec2(vector.x / d, vector.y / d);
        }

        std::vector<float> calculateGeometry(maths::vec2 light, maths::vec4 wall, float lightRadius) {
            const maths::vec2 wa = maths::vec2(wall.x, wall.y) - light;
            const maths::vec2 sa = unitVector(wa) * lightRadius;
            const maths::vec2 ea = light + sa;

            const maths::vec2 wb = maths::vec2(wall.z, wall.w) - light;
            const maths::vec2 sb = unitVector(wb) * lightRadius;
            const maths::vec2 eb = light + sb;

            std::vector<float> ans;

            /*ans.push_back(0);
            ans.push_back(0);

            ans.push_back(1);
            ans.push_back(0);

            ans.push_back(1);
            ans.push_back(-1);

            ans.push_back(0);
            ans.push_back(-1);*/

            ans.push_back(wall.x);
            ans.push_back(wall.y);

            ans.push_back(ea.x);
            ans.push_back(ea.y);

            ans.push_back(eb.x);
            ans.push_back(ea.y);

            ans.push_back(wall.z);
            ans.push_back(wall.w);

            return ans;
        }

        Shadow::Shadow(maths::vec2 light_position, maths::vec4 wall_position, float radius, unsigned int color)
        {
            m_LightPosition = light_position;
            m_WallPosition = wall_position;
            m_Radius = radius;
            m_Color = color;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        Shadow::Shadow(maths::vec2 light_position, maths::vec4 wall_position, float radius, Texture* texture)
        {
            m_LightPosition = light_position;
            m_WallPosition = wall_position;
            m_Radius = radius;
            m_Texture = texture;
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }


        void Shadow::submit(Renderer2D* renderer) const
        {
            auto a = calculateGeometry(m_LightPosition, m_WallPosition, m_Radius);
            Renderable2D* renderable2D = new Renderable2D(a, m_Texture);
            renderer->drawShadow(renderable2D);
        }
    } }