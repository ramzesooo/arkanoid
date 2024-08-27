#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <memory>
#include <bitset>
#include <array>

enum class EntityGroup
{
	balls = 0,
	players,
	tiles,
	perks,
	size
};

constexpr std::size_t entityGroupSize = static_cast<std::size_t>(EntityGroup::size);

class Manager;

class Entity
{
public:
	Entity(Manager& manager, const SDL_FRect& destRect) : m_Manager(manager), dest(destRect) {}
	virtual ~Entity(){}

	virtual void Update() {}
	virtual void Draw() {}

	const SDL_FRect& GetPos() const { return dest; }

	void Destroy() { m_IsActive = false; }
	bool IsActive() const { return m_IsActive; }

	bool HasGroup(EntityGroup group) const { return m_GroupBitSet[static_cast<std::size_t>(group)]; }
	void AddGroup(EntityGroup group);
	void DeleteGroup(EntityGroup group) { m_GroupBitSet[static_cast<std::size_t>(group)] = false; }
protected:
	SDL_FRect dest;
private:
	bool m_IsActive = true;
	Manager& m_Manager;
	std::bitset<entityGroupSize> m_GroupBitSet;
};

class Manager
{
public:
	// Refreshes vector of unique pointers to all existing Entities
	void Refresh()
	{
		for (auto i(0u); i < entityGroupSize; i++)
		{
			for (auto it = groupedEntities[i].begin(); it != groupedEntities[i].end();)
			{
				if ((*it)->IsActive())
				{
					it++;
				}
				else
				{
					it = groupedEntities[i].erase(it);
				}
			}
		}

		for (auto it = entities.begin(); it != entities.end();)
		{
			if ((*it)->IsActive())
			{
				it++;
			}
			else
			{
				it = entities.erase(it);
			}
		}
	}

	void Update()
	{
		for (const auto& e : entities)
		{
			e->Update();
		}
	}

	/*void Draw()
	{
		for (const auto& e : entities)
		{
			e->Draw();
		}
	}*/

	void AddToGroup(Entity* entity, EntityGroup group)
	{
		groupedEntities[static_cast<std::size_t>(group)].emplace_back(entity);
	}

	std::vector<Entity*>& GetGroup(EntityGroup group)
	{
		return groupedEntities[static_cast<std::size_t>(group)];
	}

	template<class T, class... Args>
	T* NewEntity(Args&&... args)
	{
		entities.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		return (T*)entities.back().get();
	}

	std::size_t GetEntitiesAmount() const { return entities.size(); }

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, entityGroupSize> groupedEntities;
};