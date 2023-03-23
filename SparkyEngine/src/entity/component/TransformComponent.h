#pragma once

#include "Component.h"

#include "../../maths/maths.h"

namespace sparky { namespace entity { namespace component {

	class TransformComponent : public Component
	{
	public:
		maths::mat4 transform;
	public:
        TransformComponent(Entity* entity, const maths::mat4& transform);

		static ComponentType* GetStaticType()
		{
			static ComponentType type({ "Transform" });
			return &type;
		}

		inline virtual ComponentType* GetType() const override { return GetStaticType(); }
	};

} } }