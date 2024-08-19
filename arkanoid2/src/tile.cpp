#include "SDL.h"
#include "assetManager.h"
#include "app.h"
#include "log.h"
#include "entity.h"
#include "tile.h"
#include <typeinfo>

Tile::Tile(const std::string& textureID, float startX, float startY) 
	: Entity(*App::s_Manager, { startX, startY, App::s_TilesWidth, 16 }), m_TextureID(textureID)
{
	//dest.x = startX;
	//dest.y = startY;
	//dest.w = App::s_TilesWidth;

	App::s_Logger->LogConstructor(typeid(*this).name());
}

void Tile::Update()
{

}

void Tile::Draw()
{
	App::s_Assets->DrawTexture(m_TextureID, Tile::tile_source, dest);
}