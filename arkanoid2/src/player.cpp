#include "player.h"
#include "app.h"
#include "assetManager.h"
#include "log.h"

Player::Player()
	: Entity({ (float)App::WINDOW_WIDTH / 2 - m_Dest.w / 2, (float)App::WINDOW_HEIGHT / 2 + (float)App::WINDOW_HEIGHT / 4, (float)Player::s_OriginalWidth, (float)Player::s_OriginalHeight })
{
	AddGroup(EntityGroup::players);

	App::s_Logger->LogConstructor(typeid(*this).name());
}

void Player::Update()
{
	if (App::s_Event.type == SDL_MOUSEMOTION)
	{
		m_Dest.x = App::s_Event.motion.x - m_Dest.w / 2;
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
	App::s_Assets->DrawTexture("player", Player::player_source, m_Dest);
}

void Player::SetAffect(PerkType perkType)
{
	//if (m_Affect == perkType)
	//{
	//	//m_EndTime = SDL_GetTicks() + App::s_AffectTime;
	//	m_EndTime += App::s_AffectTime;
	//	return;
	//}

	switch (perkType)
	{
	case PerkType::shrink:
		m_Dest.w = Player::s_OriginalWidth / 1.5f;
		break;
	case PerkType::supersize:
		// Check is the player shrinked and enlarge him accordingly to original size
		if (m_Affect == PerkType::shrink)
		{
			m_Dest.w = Player::s_OriginalWidth * 1.5f;
			break;
		}
		
		// Limit enlarging to 3 times
		if (m_Dest.w < Player::s_OriginalWidth * (1.5f * 3))
		{
			m_Dest.w *= 1.5f;
		}
		break;
	case PerkType::none:
		CancelAffect();
		return;
	default:
		break;
	}

	if (m_Affect == perkType)
	{
		m_EndTime += App::s_AffectTime;
	}
	else
	{
		m_EndTime = SDL_GetTicks() + App::s_AffectTime;
	}

	m_Affect = perkType;
}

void Player::CancelAffect()
{
	if (m_Affect == PerkType::none)
	{
		return;
	}

	m_EndTime = NULL;
	m_Affect = PerkType::none;
	m_Dest.w = Player::s_OriginalWidth;
	/*switch (m_Affect)
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
	}*/
}