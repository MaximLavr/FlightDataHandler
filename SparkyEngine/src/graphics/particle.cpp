#include "particle.h"

# define M_PI  3.14159265358979323846

namespace sparky { namespace graphics {

        float degreesToRadians(float degrees) {
            return degrees * (M_PI / 180);
        }

        float getDistanceVectors(vec2 a, vec2 b)
        {
            return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
        }

        auto min(double a, double b)
        {
            if (a < b)
                return a;
            else
                return b;
        }

        Particle::Particle(vec2 pos, float count)
        {
            m_Position = pos;
            m_Count = count;

            float a_plus = 360.0f / m_Count;
            for (float a = 0.0f; a < 360.0f; a += a_plus)
            {
                auto* ray = new Ray(m_Position, degreesToRadians(a));
                m_Rays.push_back(ray);
            }
        }

        void Particle::submit(Renderer2D *renderer) const
        {
            for (auto ray : m_Rays)
            {
                /*if (ray->isHitWall(m_Wall))
               {
                   vec2 point = ray->getWallPoint(m_Wall);
                   renderer->DrawLine(this->position,point,0xffffffff, 0.015);
               }*/
                //renderer->DrawLine(this->m_Position,
                  //                 this->m_Position + ray->getDirection(),
                    //               0xffffffff, 0.015);
                vec2 closest = NULL;
                auto record = std::numeric_limits<float>::infinity();
                for (auto wall : m_Walls)
                {
                    if (ray->isHitWall(wall))
                    {
                        vec2 point = ray->getWallPoint(wall);
                        auto d = getDistanceVectors(this->m_Position, point);
                        if (d < record)
                        {
                            record = d;
                            closest = point;
                        }
                    }
                }
                if (closest != NULL)
                    renderer->DrawLine(this->m_Position,closest,0xffffffff, 0.015);
            }
        }

        void Particle::setPosition(vec2 position)
        {
            float a_plus = 360.0f / m_Count;
            m_Position = position;
            m_Rays.clear();
            for (float a = 0.0f; a < 360.0f; a += a_plus)
            {
                auto* ray = new Ray(m_Position, degreesToRadians(a));
                m_Rays.push_back(ray);
            }
        }

    } }

