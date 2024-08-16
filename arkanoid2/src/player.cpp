#include "app.h"
#include "log.h"
#include "entity.h"
#include "player.h"

Player::Player()
{
	App::logger->LogConstructor(typeid(this).name());
}

Player::~Player()
{
	App::logger->LogDeconstructor(typeid(this).name());
}

void Player::Update()
{

}

void Player::Draw()
{

}