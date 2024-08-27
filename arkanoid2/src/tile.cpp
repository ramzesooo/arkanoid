#include "tile.h"
#include "app.h"
#include "assetManager.h"
#include "log.h"

Tile::Tile(std::string_view textureID, TileType type, float startX, float startY) 
	: Entity({ startX, startY, App::s_TilesWidth, 16 }), m_TextureID(textureID), m_Type(type)
{
	AddGroup(EntityGroup::tiles);
}

//void Tile::Update()
//{
//
//}

void Tile::Draw()
{
	App::s_Assets->DrawTexture(m_TextureID, Tile::tile_source, m_Dest);
}