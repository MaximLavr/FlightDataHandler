#pragma once

#include "Component.h"

#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/src/graphics/batchrenderer2d.h"
#include "../../maths/rectangle.h"

class b2Body;
class b2BodyDef;
class b2World;

namespace sparky {
    class Scene2D;
}

namespace sparky { namespace entity { namespace component {

            class Physics2DComponent : public Component
        {
            private:
            friend class ::sparky::Scene2D;
            public:
            b2Body* m_Body;
            b2BodyDef* m_BodyDef;
            maths::Rectangle* m_EntityBounds;
            public:
            Physics2DComponent(Entity* entity);
            ~Physics2DComponent();

            const maths::vec2& GetPosition() const;
            void SetPosition(const maths::vec2& position);

            inline b2Body* GetBody() { return m_Body; }

            void ApplyForce(const maths::vec2& force);
            void DebugRender(graphics::BatchRenderer2D& renderer);

            static ComponentType* GetStaticType()
            {
                static ComponentType type({ "Mesh" });
                return &type;
            }

            inline virtual ComponentType* GetType() const override { return GetStaticType(); }
        private:
        void Init(b2World* world);
    };

} } }