#pragma once

//#include "sp/Common.h"
//#include "sp/Types.h"
//#include "sp/String.h"
#include <iostream>

namespace sparky { namespace entity {
	class Entity;
} }

namespace sparky { namespace entity { namespace component {

	struct ComponentType
	{
		std::string name;
	};

	class Component
	{
	protected:
		Entity* m_Entity;
	public:
        Component(Entity* entity);

		virtual Entity* GetEntity() { return m_Entity; }
		virtual ComponentType* GetType() const { return nullptr; }
	};

} } }
