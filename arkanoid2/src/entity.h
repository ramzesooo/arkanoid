#pragma once
#include <vector>
#include <typeinfo>
#include <string>

class Entity
{
public:
	Entity(){}

	virtual ~Entity(){}

	virtual void Update() {}
	virtual void Draw() {}

	inline void Destroy() { m_IsActive = false; }

	inline bool IsActive() const { return m_IsActive; }
protected:
	std::string m_Tag = "";
private:
	bool m_IsActive = true;
};

class Manager
{
public:
	Manager(){}

	~Manager()
	{
		for (auto& e : entities)
		{
			delete e;
			e = nullptr;
		}

		entities.clear();
	}

	void Refresh()
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

	template<typename T>
	void AddEntity(T& entity)
	{
		entities.push_back(entity);
	}

private:
	std::vector<Entity*> entities;
};