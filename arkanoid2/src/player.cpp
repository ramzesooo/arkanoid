#include "app.h"
#include "assetManager.h"
#include "log.h"
#include "entity.h"
#include "player.h"

Player::Player() 
	: Entity(*App::s_Manager, { (float)App::WINDOW_WIDTH / 2 - dest.w / 2, (float)App::WINDOW_HEIGHT / 2 + (float)App::WINDOW_HEIGHT / 4, 64, 32 })
{
	App::s_Logger->LogConstructor(typeid(*this).name());
}

void Player::Update()
{
	if (App::s_Event.type == SDL_MOUSEMOTION)
	{
		dest.x = App::s_Event.motion.x - dest.w / 2;
	}

	switch (m_Affect)
	{
	case PerkTypes::shrink:
		if (SDL_TICKS_PASSED(SDL_GetTicks(), m_EndTime))
		{
			CancelAffect();
		}
		break;
	case PerkTypes::supersize:
		if (SDL_TICKS_PASSED(SDL_GetTicks(), m_EndTime))
		{
			CancelAffect();
		}
		break;
	case PerkTypes::none:
	default:
		break;
	}
}

void Player::Draw()
{
	App::s_Assets->DrawTexture("player", Player::player_source, dest);
}

void Player::SetAffect(PerkTypes perkType)
{
	if (m_Affect == perkType)
	{
		return;
	}

	// check if current affect affects size of the player and restore it to default
	if (m_Affect == PerkTypes::shrink)
	{
		dest.w *= 1.5f;
	}
	else if (m_Affect == PerkTypes::supersize)
	{
		dest.w /= 1.5f;
	}

	// Change size of the player appropriately to the affect
	if (perkType == PerkTypes::shrink)
	{
		dest.w /= 1.5f;
	}
	else if (perkType == PerkTypes::supersize)
	{
		dest.w *= 1.5f;
	}

	m_Affect = perkType;
	m_EndTime = SDL_GetTicks() + App::s_AffectTime;
}

void Player::CancelAffect()
{
	switch (m_Affect)
	{
	case PerkTypes::shrink:
		m_EndTime = NULL;
		m_Affect = PerkTypes::none;
		dest.w *= 1.5f;
		break;
	case PerkTypes::supersize:
		m_EndTime = NULL;
		m_Affect = PerkTypes::none;
		dest.w /= 1.5f;
		break;
	case PerkTypes::none:
	default:
		break;
	}
}