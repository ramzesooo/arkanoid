#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "tile.h"
#include <typeinfo>

Tile::Tile(const std::string& textureID, float startX, float startY) : m_TextureID(textureID)
{
	m_Tag = entity_type::tile;

	dest.x = startX;
	dest.y = startY;

	App::logger->LogConstructor(typeid(*this).name());
}

Tile::~Tile()
{
	App::logger->LogDeconstructor(typeid(*this).name());
}

void Tile::Update()
{

}

void Tile::Draw()
{
	App::assets->DrawTexture(m_TextureID, src, dest);
}