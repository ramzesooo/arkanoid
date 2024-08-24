#pragma once
#include "entity.h"

struct Velocity
{
	float x = 0.0f, y = 0.0f;
};

class Ball : public Entity
{
public:
	Ball(float startX, float startY, Velocity startVelocity);

	void Update() override;
	void Draw() override;

	Velocity& GetVelocity() { return velocity; }

	void HitPlayer(const SDL_FRect& playerPos);
	void HitTile(const SDL_FRect& tilePos);
private:
	static constexpr SDL_Rect ball_source{ 0, 0, 256, 256 };
	Velocity velocity{ 0.0f, 0.0f };
};