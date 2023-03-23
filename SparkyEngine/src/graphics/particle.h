#pragma once
#include "../maths/maths.h"
#include "ray.h"
#include "renderable2d.h"
#include <math.h>
#include "layers/layer.h"
#include "../utils/colorTransformer.h"

using namespace sparky::maths;

namespace sparky { namespace graphics {

        class Particle : public Renderable2D
        {
        public:
            Particle(vec2 pos, float ct);
            void submit(Renderer2D* renderer) const override;
            vec2 m_Position;
            float m_Count;
            void setSprite(const std::vector<vec4>& walls)
            {
                for (auto wall : walls)
                {
                    m_Walls.push_back(wall);
                }
            }
            void setWall(vec4 wall) { m_Wall = wall; }
            void setPosition(vec2 position);
            vec2 getPosition() { return m_Position; }
        private:
            vec4 m_Wall;
            std::vector<vec4> m_Walls;
            std::vector<Ray*> m_Rays;
        };

} }