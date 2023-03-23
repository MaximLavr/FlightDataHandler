#pragma once

#include "Component.h"

//#include "sp/Common.h"
#include "../../graphics/sprite.h"

namespace sparky { namespace entity { namespace component {

	class SpriteComponent : public Component
	{
    public:
        graphics::Sprite* sprite;
    public:
        SpriteComponent(Entity* entity, graphics::Sprite* sprite);

        static ComponentType* GetStaticType()
        {
            static ComponentType type({ "Sprite" });
            return &type;
        }

        inline virtual ComponentType* GetType() const override { return GetStaticType(); }
	};

} } }

