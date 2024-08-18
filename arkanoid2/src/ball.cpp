#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball(float startX, float startY, const std::string& textureID, uint16_t ballID, Velocity startVelocity) : m_TextureID(textureID), m_BallID(ballID), velocity(startVelocity)
{
	m_Tag = "ball";

	dest.x = startX;
	dest.y = startY;

	App::ballsCount++;

	App::logger->LogConstructor(typeid(*this).name());
}

Ball::~Ball()
{
	App::ballsCount--;
	App::logger->LogDeconstructor(typeid(*this).name());
}

void Ball::Draw()
{
	App::assets->DrawTexture(m_TextureID, src, dest);
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

void Ball::AABB(const SDL_Rect& entityPos)
{
	// primary statement for checking did the collision happen
	if (dest.y + dest.h / 2 >= entityPos.y
		&& dest.y <= entityPos.y + entityPos.h
		&& dest.x + dest.w / 2 >= entityPos.x
		&& dest.x <= entityPos.x + entityPos.w)
	{
		float centerX = static_cast<float>(entityPos.x + entityPos.w / 2);
		float centerY = static_cast<float>(entityPos.y + entityPos.h / 2);

		float distanceX = ((dest.x + dest.w / 2) - centerX) / (static_cast<float>(entityPos.w) / 2);
		float distanceY = 0.0f;

		velocity.x = distanceX * App::MaxSpeedX;

		velocity.y = -std::abs(velocity.y);

		if (std::abs(velocity.y) < App::MinSpeedY)
		{
			velocity.y = -App::MinSpeedY;
		}

		App::logger->Print(typeid(*this).name(), std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
	}
}