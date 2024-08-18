#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball(float startX, float startY, Velocity startVelocity) : velocity(startVelocity)
{
	m_Tag = entity_type::ball;

	dest.x = startX;
	dest.y = startY;

	App::logger->LogConstructor(typeid(*this).name());
}

Ball::~Ball()
{
	App::logger->LogDeconstructor(typeid(*this).name());
}

void Ball::Draw()
{
	App::assets->DrawTexture("defaultBall", src, dest);
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