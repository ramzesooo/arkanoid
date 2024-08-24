#pragma once
#include "entity.h"

#include "SDL.h"
#include <string>

class Tile : public Entity
{
public:
	Tile(std::string_view textureID, float startX, float startY);

	void Update() override;
	void Draw() override;
private:
	std::string m_TextureID = "";
	static constexpr SDL_Rect tile_source{ 0, 0, 64, 32 };
};