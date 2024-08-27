#include "tile.h"
#include "assetManager.h"
#include "log.h"
#include "app.h"

Tile::Tile(std::string_view textureID, TileType type, float startX, float startY) 
	: Entity(*App::s_Manager, { startX, startY, App::s_TilesWidth, 16 }), m_TextureID(textureID), m_Type(type)
{
	AddGroup(EntityGroup::tiles);
}

//void Tile::Update()
//{
//
//}

void Tile::Draw()
{
	App::s_Assets->DrawTexture(m_TextureID, Tile::tile_source, dest);
}