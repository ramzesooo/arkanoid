#include "SDL.h"
#include "SDL_image.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball(float startX, float startY, const std::string& textureID, uint16_t ballID) : m_TextureID(textureID), m_ballID(ballID)
{
	dest.x = startX;
	dest.y = startY;

	velocity.x = velocity.y = App::Speed;

	App::logger->LogConstructor(typeid(*this).name());
}

Ball::~Ball()
{
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
		velocity.x = -(App::Speed);
	}

	if (dest.y + dest.h > screenH)
	{
		velocity.y = -(App::Speed);
	}

	if (dest.x <= 0)
	{
		velocity.x = App::Speed;
	}

	if (dest.y < 0)
	{
		velocity.y = App::Speed;
	}

	dest.x += velocity.x;
	dest.y += velocity.y;
}