#pragma once

class Player : public Entity
{
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;
private:
	SDL_Rect src{ 0, 0, 64, 32 };
	SDL_Rect dest{ 0, 0, 64, 32 };
};