#include "SDL.h"
#include "SDL_image.h"
#include "assetManager.h"
#include "app.h"
#include <typeinfo>
#include "log.h"

AssetManager::AssetManager()
{
	App::logger->LogConstructor(typeid(this).name());
}

AssetManager::~AssetManager()
{
	for (auto& texture : textures)
	{
		SDL_DestroyTexture(texture.second);
		std::string output = "Destroyed texture ";
		output += texture.first;
		App::logger->Print(typeid(this).name(), output);
	}

	App::logger->LogDeconstructor(typeid(this).name());
}

void AssetManager::LoadTexture(const std::string& textureID, const std::string& path)
{
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (!tempSurface)
	{
		App::logger->LogSDL("LoadTexture (missing tempSurface): ");
		return;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(App::renderer, tempSurface);
	if (!texture)
	{
		App::logger->LogSDL("LoadTexture (missing texture from surface): ");
		return;
	}

	textures.emplace(textureID, texture);
	SDL_FreeSurface(tempSurface);

	{
		std::string output = "LoadTexture: Loaded ";
		output += textureID + " (" + path + ")";
		App::logger->Debug(typeid(this).name(), output);
	}
}

void AssetManager::DrawTexture(const std::string& textureID)
{

}