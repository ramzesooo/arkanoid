#pragma once
#include <vector>
#include <typeinfo>
#include <string>
#include <memory>

enum class entity_type
{
	player = 0,
	ball,
	tile
};

class Entity
{
public:
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

	/*void Refresh()
	{
		std::vector<std::vector<Entity*>::iterator> garbage; // vector of entities iterators to erase from the vector

		for (auto it = entities.begin(); it != entities.end(); it++)
		{
			if (!(*it)->IsActive())
			{
				delete* it;
				*it = nullptr;
				garbage.push_back(it);
			}
		}

		for (auto it = garbage.rbegin(); it != garbage.rend(); it++)
		{
			entities.erase(*it);
		}
	}*/

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

	/*template<typename T>
	void AddEntity(T& entity)
	{
		entities.push_back(entity);
	}*/

	template<class T, class... Args>
	Entity* NewEntity(Args&&... args)
	{
		entities.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		return entities.back().get();
	}

private:
	//std::vector<Entity*> entities;
	std::vector<std::unique_ptr<Entity>> entities;
};