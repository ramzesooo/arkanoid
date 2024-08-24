#include "player.h"
#include "app.h"
#include "assetManager.h"
#include "log.h"

Player::Player() 
	: Entity(*App::s_Manager, { (float)App::WINDOW_WIDTH / 2 - dest.w / 2, (float)App::WINDOW_HEIGHT / 2 + (float)App::WINDOW_HEIGHT / 4, 64, 32 })
{
	AddGroup(EntityGroup::players);

	App::s_Logger->LogConstructor(typeid(*this).name());
}

void Player::Update()
{
	if (App::s_Event.type == SDL_MOUSEMOTION)
	{
		dest.x = App::s_Event.motion.x - dest.w / 2;
	}

	if (m_Affect == PerkType::none)
	{
		return;
	}

	switch (m_Affect)
	{
	case PerkType::shrink:
		if (SDL_TICKS_PASSED(SDL_GetTicks(), m_EndTime))
		{
			CancelAffect();
		}
		break;
	case PerkType::supersize:
		if (SDL_TICKS_PASSED(SDL_GetTicks(), m_EndTime))
		{
			CancelAffect();
		}
		break;
	case PerkType::none:
	default:
		break;
	}
}

void Player::Draw()
{
	App::s_Assets->DrawTexture("player", Player::player_source, dest);
}

void Player::SetAffect(PerkType perkType)
{
	if (m_Affect == perkType)
	{
		m_EndTime = SDL_GetTicks() + App::s_AffectTime;
		return;
	}

	switch (perkType)
	{
	case PerkType::shrink:
		if (m_Affect == PerkType::supersize)
		{
			dest.w /= 2.25f;
		}
		else
		{
			dest.w /= 1.5f;
		}
		break;
	case PerkType::supersize:
		if (m_Affect == PerkType::shrink)
		{
			dest.w *= 2.25f;
		}
		else
		{
			dest.w *= 1.5f;
		}
		break;
	case PerkType::none:
	default:
		break;
	}

	// check if current affect affects size of the player and restore it to default
	//if (m_Affect == PerkType::shrink)
	//{
	//	dest.w *= 1.5f;
	//}
	//else if (m_Affect == PerkType::supersize)
	//{
	//	dest.w /= 1.5f;
	//}

	//// Change size of the player appropriately to the affect
	//if (perkType == PerkType::shrink)
	//{
	//	dest.w /= 1.5f;
	//}
	//else if (perkType == PerkType::supersize)
	//{
	//	dest.w *= 1.5f;
	//}

	m_Affect = perkType;
	m_EndTime = SDL_GetTicks() + App::s_AffectTime;
}

void Player::CancelAffect()
{
	switch (m_Affect)
	{
	case PerkType::shrink:
		m_EndTime = NULL;
		m_Affect = PerkType::none;
		dest.w *= 1.5f;
		break;
	case PerkType::supersize:
		m_EndTime = NULL;
		m_Affect = PerkType::none;
		dest.w /= 1.5f;
		break;
	case PerkType::none:
	default:
		break;
	}
}