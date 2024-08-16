#include "SDL.h"
#include "SDL_image.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball(uint32_t startX, uint32_t startY, const std::string& textureID, uint16_t ballID) : m_TextureID(textureID), m_ballID(ballID)
{
	dest.x = startX;
	dest.y = startY;

	App::logger->LogConstructor(typeid(this).name());
}

Ball::~Ball()
{
	App::logger->LogDeconstructor(typeid(this).name());
}

void Ball::Draw()
{
	App::assets->DrawTexture(m_TextureID, src, dest);
}

void Ball::Update()
{

}