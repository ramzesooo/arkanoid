#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "perk.h"
#include <typeinfo>

Perk::Perk(const std::string& textureID, float startX, float startY, PerkTypes perkType)
	: Entity(*App::s_Manager, { startX, startY, 20.0f, 20.0f }), m_TextureID(textureID), m_PerkType(perkType)
{
	App::s_Logger->LogConstructor(typeid(*this).name());
}

Perk::~Perk()
{
	App::s_Logger->LogDestructor(typeid(*this).name());
}

void Perk::Update()
{
	if (dest.y >= (float)App::WINDOW_HEIGHT)
	{
		Destroy();
	}
	else
	{
		dest.y += 0.5f;
	}
}

void Perk::Draw()
{
	App::s_Assets->DrawTexture(m_TextureID, Perk::perk_source, dest);
}