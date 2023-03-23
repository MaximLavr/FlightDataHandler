//#include "../../sparky.h"
#include "SpriteComponent.h"

namespace sparky { namespace entity { namespace component {

	SpriteComponent::SpriteComponent(Entity* entity, graphics::Sprite* sprite)
		: Component(entity), sprite(sprite)
	{
	}

} } }