#include "entity.h"
#include "app.h"

Entity::Entity(const SDL_FRect& destRect) : m_Dest(destRect), m_Manager(App::s_Manager.get())
{}

void Entity::AddGroup(EntityGroup group)
{
	m_GroupBitSet[static_cast<std::size_t>(group)] = true;
	m_Manager->AddToGroup(this, group);
}