#pragma once
#include <vector>
#include <typeinfo>
#include <string>
#include <memory>
#include <bitset>
#include <array>

constexpr std::size_t maxGroups = 8;

class Manager;

class Entity
{
public:
	Entity(Manager& manager, const SDL_FRect& destRect) : m_Manager(manager), dest(destRect) {}
	virtual ~Entity(){}

	virtual void Update() {}
	virtual void Draw() {}

	const SDL_FRect& GetPos() { return dest; }

	void Destroy() { m_IsActive = false; }

	bool IsActive() const { return m_IsActive; }

	bool HasGroup(std::size_t group) const { return m_GroupBitSet[group]; }
	void AddGroup(std::size_t group);
	void DeleteGroup(std::size_t group) { m_GroupBitSet[group] = false; }
protected:
	SDL_FRect dest{ .0f, .0f, .0f, .0f };
private:
	bool m_IsActive = true;
	Manager& m_Manager;
	std::bitset<maxGroups> m_GroupBitSet;
};

class Manager
{
public:
	// Refreshes vector of unique pointers to all existing Entities
	void Refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
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

	void Draw()
	{
		for (const auto& e : entities)
		{
			e->Draw();
		}
	}

	void AddToGroup(Entity* entity, std::size_t group)
	{
		groupedEntities[group].emplace_back(entity);
	}

	std::vector<Entity*>& GetGroup(std::size_t group)
	{
		return groupedEntities[group];
	}

	template<class T, class... Args>
	T* NewEntity(Args&&... args)
	{
		entities.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		return (T*)entities.back().get();
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
};