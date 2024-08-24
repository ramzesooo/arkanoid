#include "entity.h"

void Entity::AddGroup(EntityGroup group)
{
	m_GroupBitSet[static_cast<std::size_t>(group)] = true;
	m_Manager.AddToGroup(this, group);
}