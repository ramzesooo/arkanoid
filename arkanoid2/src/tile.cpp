#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "tile.h"
#include <typeinfo>

Tile::Tile(const std::string& textureID, uint32_t startX, uint32_t startY) : m_TextureID(textureID)
{
	App::tilesCount++;

	m_Tag = "tile";

	m_TileID = App::tilesCount;

	dest.x = startX;
	dest.y = startY;

	App::logger->LogConstructor(typeid(*this).name());
}

Tile::~Tile()
{
	App::tilesCount--;

	App::logger->LogDeconstructor(typeid(*this).name());
}

void Tile::Update()
{

}

void Tile::Draw()
{
	App::assets->DrawTexture(m_TextureID, src, dest);
}