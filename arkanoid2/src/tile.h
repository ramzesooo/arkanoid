#pragma once
#include <string>

class Entity;

class Tile : public Entity
{
public:
	Tile(const std::string& textureID, float startX, float startY);
	~Tile();

	void Update() override;
	void Draw() override;
private:
	std::string m_TextureID = "";
	SDL_Rect src{ 0, 0, 64, 32 };
	SDL_FRect dest{ 0, 0, 64, 16 };
};