#include "SDL.h"
#include "entity.h"

void Entity::AddGroup(std::size_t group)
{
	m_GroupBitSet[group] = true;
	m_Manager.AddToGroup(this, group);
}