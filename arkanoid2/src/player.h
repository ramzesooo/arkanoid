#pragma once

class Player : public Entity
{
public:
	Player();
	~Player();

	void Update() override;
	void Draw() override;

	const SDL_FRect& GetPos() { return dest; }
private:
	SDL_Rect src{ 0, 0, 64, 32 };
	SDL_FRect dest{ 0, 0, 64, 32 };
};