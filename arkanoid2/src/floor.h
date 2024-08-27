#pragma once
#include "entity.h"

#include "SDL.h"

class Floor : public Entity
{
public:
	Floor();

	void Update() override;
	void Draw() override;

	void Toggle();
	void Extend();
	bool IsShown() const { return m_Shown; }
private:
	static constexpr SDL_Rect floor_source{ 0, 0, 64, 64 };
	bool m_Shown = false;
	uint32_t m_EndTime = NULL;
};