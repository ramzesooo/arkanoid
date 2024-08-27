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
	Tile(std::string_view textureID, TileType type, float startX, float startY);

	//void Update() override;
	void Draw() override;

	const TileType& GetType() const { return m_Type; }
private:
	TileType m_Type;
	std::string_view m_TextureID;
	static constexpr SDL_Rect tile_source{ 0, 0, 64, 32 };
};