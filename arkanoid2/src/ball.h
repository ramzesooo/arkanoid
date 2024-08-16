#pragma once

class Ball : public Entity
{
public:
	Ball();
	~Ball();

	void Update() override;
	void Draw() override;
private:
	SDL_Texture* texture = nullptr;
};