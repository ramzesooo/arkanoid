#pragma once

struct Velocity
{
	float x = 0.0f, y = 0.0f;
};

class Ball : public Entity
{
public:
	Ball(float startX, float startY, Velocity startVelocity);
	~Ball();

	void Update() override;
	void Draw() override;

	Velocity& GetVelocity() { return velocity; }
	const SDL_FRect& GetPos() { return dest; }

	//inline uint16_t GetBallID() const { return m_BallID; }
private:
	SDL_Rect src{ 0, 0, 256, 256 };
	SDL_FRect dest{ 0.0f, 0.0f, 12.0f, 12.0f };
	Velocity velocity{ 0.0f, 0.0f };
};