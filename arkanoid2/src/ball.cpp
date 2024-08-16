#include "SDL.h"
#include "SDL_image.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "ball.h"

Ball::Ball()
{
	App::logger->LogConstructor(typeid(this).name());
}

Ball::~Ball()
{
	App::logger->LogDeconstructor(typeid(this).name());
}

void Ball::Draw()
{
	
}

void Ball::Update()
{

}