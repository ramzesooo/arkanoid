#pragma once
#include <string>

class Entity;

class Tile : public Entity
{
public:
	Tile(const std::string& textureID, float startX, float startY);

	void Update() override;
	void Draw() override;
private:
	std::string m_TextureID = "";
	static constexpr SDL_Rect tile_source{ 0, 0, 64, 32 };
};