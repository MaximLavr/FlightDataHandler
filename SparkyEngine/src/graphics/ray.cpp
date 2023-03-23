#include "ray.h"

namespace sparky { namespace graphics {

        Ray::Ray(vec2 pos, float angle)
        {
            m_Position = pos;
            float dirX, dirY;

            dirX =  (1) * cos(angle) - (1) * sin(angle);
            dirY =  (1) * sin(angle) + (1) * cos(angle);

            m_Direction = vec2(dirX, dirY);
        }

        void Ray::submit(Renderer2D* renderer) const
        {
            renderer->DrawLine(vec2(m_Position.x, m_Position.y),
                               vec2(m_Position.x+m_Direction.x, m_Position.y + m_Direction.y),
                               0xffffffff, 0.006);
        }

        bool Ray::isHitWall(vec4 wall)
        {
            float x1 = wall.x;
            float y1 = wall.y;
            float x2 = wall.z;
            float y2 = wall.w;

            float x3 = this->m_Position.x;
            float y3 = this->m_Position.y;
            float x4 = this->m_Position.x + this->m_Direction.x;
            float y4 = this->m_Position.y + this->m_Direction.y;

            float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
            if (den == 0)
                return false;
            float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
            float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
            m_T = t;


            if (t > 0 && t < 1 && u > 0)
            {
                return true;
            }
            else
                return false;
        }

        vec2 Ray::getWallPoint(vec4 wall)
        {
            vec2 point;
            point.x = wall.x + m_T * (wall.z - wall.x);
            point.y = wall.y + m_T * (wall.w - wall.y);

            return point;
        }

    } }

