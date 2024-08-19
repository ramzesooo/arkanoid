#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball(float startX, float startY, Velocity startVelocity) : Entity(entity_type::ball), velocity(startVelocity)
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