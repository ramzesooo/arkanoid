#pragma once
#include <string>

class Entity;

class Tile : public Entity
{
public:
	Tile(const std::string& textureID, uint32_t startX, uint32_t startY);
	~Tile();

	void Update() override;
	void Draw() override;

	inline uint16_t GetTileID() const { return m_TileID; }
private:
	uint16_t m_TileID = 0;
	std::string m_TextureID = "";
	SDL_Rect src{ 0, 0, 64, 32 };
	SDL_Rect dest{ 0, 0, 64, 16 };
};