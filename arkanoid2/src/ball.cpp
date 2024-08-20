#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball(float startX, float startY, Velocity startVelocity) 
	: Entity(*App::s_Manager, { startX, startY, 12.0f, 12.0f }), velocity(startVelocity)
{
	dest.x = startX;
	dest.y = startY;

	App::s_Logger->LogConstructor(typeid(*this).name());
}

void Ball::Draw()
{
	App::s_Assets->DrawTexture("defaultBall", Ball::ball_source, dest);
}

void Ball::Update()
{
	uint32_t screenW = App::WINDOW_WIDTH;
	uint32_t screenH = App::WINDOW_HEIGHT;

	if (dest.x + dest.w > screenW)
	{
		velocity.x = -velocity.x;
	}

	if (dest.y >= screenH)
	{
		Destroy();
		return;
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
	float centerY = playerPos.y + playerPos.h / 2;

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
	float centerY = tilePos.y + tilePos.h / 2;

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