#pragma once
#include <vector>
#include <typeinfo>
#include <string>
#include <memory>

enum class entity_type
{
	ball = 0,
	player,
	tile
};

class Entity
{
public:
	Entity(entity_type tag) : m_Tag(tag){}
	virtual ~Entity(){}

	virtual void Update() {}
	virtual void Draw() {}

	void Destroy() { m_IsActive = false; }

	bool IsActive() const { return m_IsActive; }
protected:
	entity_type m_Tag;
private:
	bool m_IsActive = true;
};

class Manager
{
public:
	// Refreshes vector of unique pointers to all existing Entities
	void Refresh()
	{
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

	template<class T, class... Args>
	Entity* NewEntity(Args&&... args)
	{
		entities.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		return entities.back().get();
	}

private:
	std::vector<std::unique_ptr<Entity>> entities;
};