#include "vastpch.h"
#include "Entity.h"

namespace Vast {

	Entity::Entity(EntityID handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
	}

}