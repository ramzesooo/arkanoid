#pragma once

struct Velocity
{
	float x = 0.0f, y = 0.0f;
};

class Ball : public Entity
{
public:
	Ball(float startX, float startY, const std::string& textureID, uint16_t ballID);
	~Ball();

	void Update() override;
	void Draw() override;

	inline uint16_t GetBallID() const { return m_ballID; }
private:
	std::string m_TextureID = "";
	uint16_t m_ballID = 0;
	SDL_Rect src{ 0, 0, 256, 256 };
	SDL_FRect dest{ 0, 0, 12, 12 };
	Velocity velocity{ 0.0f, 0.0f };
};