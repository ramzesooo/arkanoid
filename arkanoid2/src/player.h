#pragma once

class Player : public Entity
{
public:
	Player();

	void Update() override;
	void Draw() override;

	const SDL_FRect& GetPos() { return dest; }
private:
	static constexpr SDL_Rect player_source{ 0, 0, 64, 32 };
	SDL_FRect dest{ 0, 0, 64, 32 };
};