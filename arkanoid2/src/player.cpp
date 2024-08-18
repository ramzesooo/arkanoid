#include "app.h"
#include "assetManager.h"
#include "log.h"
#include "entity.h"
#include "player.h"

Player::Player()
{
	m_Tag = entity_type::player;

	dest.x = static_cast<float>(App::WINDOW_WIDTH / 2 - dest.w / 2);
	dest.y = static_cast<float>(App::WINDOW_HEIGHT / 2 + App::WINDOW_HEIGHT / 4);

	App::logger->LogConstructor(typeid(*this).name());
}

Player::~Player()
{
	App::logger->LogDeconstructor(typeid(*this).name());
}

void Player::Update()
{
	if (App::event.type == SDL_MOUSEMOTION)
	{
		dest.x = App::event.motion.x - dest.w / 2;
	}
}

void Player::Draw()
{
	App::assets->DrawTexture("player", src, dest);
}