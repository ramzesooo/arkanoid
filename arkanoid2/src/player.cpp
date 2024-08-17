#include "app.h"
#include "assetManager.h"
#include "log.h"
#include "entity.h"
#include "player.h"

Player::Player()
{
	dest.x = App::WINDOW_WIDTH / 2 - dest.w / 2;
	dest.y = App::WINDOW_HEIGHT / 2 + App::WINDOW_HEIGHT / 4;

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
		int32_t mouseX = App::event.motion.x;

		/*if (mouseX >= App::WINDOW_WIDTH + dest.w / 2)
		{
			mouseX = App::WINDOW_WIDTH - dest.w / 2;
		}
		else if (mouseX <= 0)
		{
			mouseX = -(dest.w / 2);
		}
		else
		{
			mouseX -= dest.w / 2;
		}*/

		dest.x = mouseX - dest.w / 2;
	}
}

void Player::Draw()
{
	App::assets->DrawTexture("player", src, dest);
}