#include "ball.h"
#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"

Ball::Ball(float startX, float startY, Velocity startVelocity) 
	: Entity(*App::s_Manager, { startX, startY, 12.0f, 12.0f }), velocity(startVelocity)
{
	dest.x = startX;
	dest.y = startY;

	AddGroup(EntityGroup::balls);
}

void Ball::Draw()
{
	App::s_Assets->DrawTexture("defaultBall", Ball::ball_source, dest);
}

void Ball::Update()
{
	if (dest.y >= App::WINDOW_HEIGHT)
	{
		Destroy();
		return;
	}

	if (dest.x + dest.w > App::WINDOW_WIDTH)
	{
		velocity.x = -velocity.x;
	}

	if (dest.x <= 0)
	{
		velocity.x = -velocity.x;
	}

	if (dest.y < 0)
	{
		velocity.y = -velocity.y;
	}

	dest.x += velocity.x;
	dest.y += velocity.y;
}

void Ball::HitPlayer(const SDL_FRect& playerPos)
{
	float centerX = playerPos.x + playerPos.w / 2;

	// distance from the center of player
	float distanceX = ((dest.x + dest.w / 2) - centerX) / (playerPos.w / 2);

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
	float distanceX = ((dest.x + dest.w / 2) - centerX) / (tilePos.w / 2);

	velocity.x = distanceX * App::s_MaxSpeedX;

	velocity.y = -(-std::abs(velocity.y));

	if (std::abs(velocity.y) < App::s_MinSpeedY)
	{
		velocity.y = -(-App::s_MinSpeedY);
	}

	//App::s_Logger->Print(typeid(*this).name(), std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
}