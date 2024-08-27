#pragma once
#include "entity.h"

#include "SDL.h"
#include <string>

enum class TileType
{
	none = 0,
	wall,
	green,
	blue
};

class Tile : public Entity
{
public:
	Tile(std::string_view textureID, float startX, float startY);

	//void Update() override;
	void Draw() override;
private:
	std::string_view m_TextureID;
	static constexpr SDL_Rect tile_source{ 0, 0, 64, 32 };
};