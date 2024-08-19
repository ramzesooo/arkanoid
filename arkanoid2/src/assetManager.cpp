#include "SDL.h"
#include "SDL_image.h"
#include "assetManager.h"
#include "app.h"
#include <typeinfo>
#include "log.h"

AssetManager::AssetManager()
{
	App::s_Logger->LogConstructor(typeid(*this).name());
}

AssetManager::~AssetManager()
{
	for (auto& texture : textures)
	{
		SDL_DestroyTexture(texture.second);
		std::string output = "Destroyed texture ";
		output += texture.first;
		App::s_Logger->Print(typeid(*this).name(), output);
	}

	App::s_Logger->LogDestructor(typeid(*this).name());
}

void AssetManager::LoadTexture(const std::string& textureID, const std::string& path)
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

	textures.emplace(textureID, texture);
	SDL_FreeSurface(tempSurface);

	{
		std::string output = "LoadTexture: Loaded ";
		output += textureID + " (" + path + ")";
		App::s_Logger->Debug(typeid(*this).name(), output);
	}
}

void AssetManager::DrawTexture(const std::string& textureID, const SDL_Rect& src, const SDL_FRect& dest)
{
	if (SDL_RenderCopyExF(App::s_Renderer, textures[textureID], &src, &dest, NULL, NULL, SDL_FLIP_NONE) != 0)
	{
		App::s_Logger->LogSDL("DrawTexture: ");
	}
}