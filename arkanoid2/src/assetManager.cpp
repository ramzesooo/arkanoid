#include "assetManager.h"
#include "app.h"
#include "log.h"

#include "SDL_image.h"

AssetManager::~AssetManager()
{
	for (auto& texture : textures)
	{
		App::s_Logger->Print(typeid(*this).name(), std::string("Destroying texture ") + texture.first);
		SDL_DestroyTexture(texture.second);
	}

	textures.clear();

	App::s_Logger->LogDestructor(typeid(*this).name());
}

//void AssetManager::LoadTexture(const std::string& textureID, const std::string& path)
//{
//	SDL_Surface* tempSurface = IMG_Load(path.c_str());
//	if (!tempSurface)
//	{
//		App::s_Logger->LogSDL("LoadTexture (missing tempSurface): ");
//		return;
//	}
//
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(App::s_Renderer, tempSurface);
//	if (!texture)
//	{
//		App::s_Logger->LogSDL("LoadTexture (missing texture from surface): ");
//		return;
//	}
//	textures.emplace(textureID, texture);
//
//	SDL_FreeSurface(tempSurface);
//
//	App::s_Logger->Debug(typeid(*this).name(), std::string("LoadTexture: Loaded " + textureID + " (" + path + ")"));
//}

void AssetManager::LoadTexture(std::string_view textureID, const std::string& path)
{
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (!tempSurface)
	{
		App::s_Logger->LogSDL("LoadTexture (missing tempSurface): ");
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(App::s_Renderer, tempSurface);
	if (!texture)
	{
		App::s_Logger->LogSDL("LoadTexture (missing texture from surface): ");
		return;
	}

	textures.emplace(std::string(textureID), texture);

	SDL_FreeSurface(tempSurface);
}

void AssetManager::DrawTexture(std::string_view textureID, const SDL_Rect& src, const SDL_FRect& dest)
{
	auto it = textures.find(textureID);
	if (it == textures.end())
	{
		App::s_Logger->Print(typeid(*this).name(), std::string("Couldn't find texture ") + std::string(textureID));
		return;
	}

	// returns 0 on success
	if (SDL_RenderCopyExF(App::s_Renderer, it->second, &src, &dest, NULL, NULL, SDL_FLIP_NONE) != 0)
	{
		App::s_Logger->LogSDL("DrawTexture: ");
	}
}