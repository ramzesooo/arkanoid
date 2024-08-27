#include "floor.h"
#include "app.h"
#include "assetManager.h"

Floor::Floor()
	: Entity({ 0.0f, float(App::WINDOW_HEIGHT - float(Floor::floor_source.h / 3)), (float)App::WINDOW_WIDTH, ((float)Floor::floor_source.h / 3) - 1.0f })
{}

void Floor::Update()
{
	if (m_Shown && SDL_TICKS_PASSED(SDL_GetTicks(), m_EndTime))
	{
		Toggle();
	}
}

void Floor::Draw()
{
	if (m_Shown)
	{
		App::s_Assets->DrawTexture("floorSquare", Floor::floor_source, m_Dest);
	}
}

void Floor::Toggle()
{
	if (m_Shown)
	{
		m_Shown = false;
		m_EndTime = NULL;
	}
	else
	{
		m_Shown = true;
		m_EndTime = SDL_GetTicks() + App::s_AffectTime;
	}
}

void Floor::Extend()
{
	m_EndTime += App::s_AffectTime;
}