#include "perk.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"

#include "SDL.h"

Perk::Perk(std::string_view textureID, float startX, float startY, PerkType perkType)
	: Entity(*App::s_Manager, { startX, startY, 20.0f, 20.0f }), m_TextureID(textureID), m_PerkType(perkType)
{
	AddGroup(EntityGroup::perks);
}

void Perk::Update()
{
	dest.y += 0.3f;

	if (dest.y >= (float)App::WINDOW_HEIGHT)
	{
		Destroy();
	}
}

void Perk::Draw()
{
	App::s_Assets->DrawTexture(m_TextureID, Perk::perk_source, dest);
}