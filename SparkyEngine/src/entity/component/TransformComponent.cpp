//#include "../../sparky.h"
#include "TransformComponent.h"

namespace sparky { namespace entity { namespace component {

	TransformComponent::TransformComponent(Entity* entity, const maths::mat4& transform)
            : Component(entity), transform(transform)
	{
	}

} } }