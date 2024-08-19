#include "app.h"
#include "assetManager.h"
#include "log.h"
#include "entity.h"
#include "player.h"

Player::Player() 
	: Entity(*App::s_Manager, { (float)App::WINDOW_WIDTH / 2 - dest.w / 2, (float)App::WINDOW_HEIGHT / 2 + (float)App::WINDOW_HEIGHT / 4, 64, 32 })
{
	//dest.x = (float)App::WINDOW_WIDTH / 2 - dest.w / 2;
	//dest.y = (float)App::WINDOW_HEIGHT / 2 + (float)App::WINDOW_HEIGHT / 4;

	App::s_Logger->LogConstructor(typeid(*this).name());
}

void Player::Update()
{
	if (App::s_Event.type == SDL_MOUSEMOTION)
	{
		dest.x = App::s_Event.motion.x - dest.w / 2;
	}
}

void Player::Draw()
{
	App::s_Assets->DrawTexture("player", Player::player_source, dest);
}