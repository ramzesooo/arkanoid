#include "ball.h"
#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"

Ball::Ball(float startX, float startY, float velocityX, float velocityY)
	: Entity({ startX, startY, Ball::s_OriginalWidth, Ball::s_OriginalHeight }), velocity{ velocityX, velocityY }
{
	m_Dest.x = startX;
	m_Dest.y = startY;

	AddGroup(EntityGroup::balls);
}

void Ball::Draw()
{
	App::s_Assets->DrawTexture("defaultBall", Ball::ball_source, m_Dest);
}

void Ball::Update()
{
	if (m_Dest.y >= App::WINDOW_HEIGHT || velocity.y == 0.0f)
	{
		Destroy();
		return;
	}

	if (m_Dest.x + m_Dest.w > App::WINDOW_WIDTH)
	{
		velocity.x = -velocity.x;
	}

	if (m_Dest.x <= 0)
	{
		velocity.x = -velocity.x;
	}

	if (m_Dest.y < 0)
	{
		velocity.y = -velocity.y;
	}

	m_Dest.x += velocity.x;
	m_Dest.y += velocity.y;
}

void Ball::HitPlayer(const SDL_FRect& playerPos)
{
	float centerX = playerPos.x + playerPos.w / 2;

	// distance from the center of player
	float distanceX = ((m_Dest.x + m_Dest.w / 2) - centerX) / (playerPos.w / 2);

	velocity.x = distanceX * App::s_MaxSpeedX;

	velocity.y = -std::abs(velocity.y);

	if (std::abs(velocity.y) < App::s_MinSpeedY)
	{
		velocity.y = -App::s_MinSpeedY;
	}

	//App::s_Logger->Print(typeid(*this).name(), std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
}

void Ball::HitTile(const SDL_FRect& tilePos)
{
	float centerX = tilePos.x + tilePos.w / 2;

	// distance from the center of player
	float distanceX = ((m_Dest.x + m_Dest.w / 2) - centerX) / (tilePos.w / 2);

	velocity.x = distanceX * App::s_MaxSpeedX;

	velocity.y = -(-std::abs(velocity.y));

	if (std::abs(velocity.y) < App::s_MinSpeedY)
	{
		velocity.y = -(-App::s_MinSpeedY);
	}

	//App::s_Logger->Print(typeid(*this).name(), std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
}