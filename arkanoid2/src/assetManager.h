#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <memory>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	void LoadTexture(const std::string& textureID, const std::string& path);
	void DrawTexture(const std::string& textureID, const SDL_Rect& src, const SDL_FRect& dest);
private:
	std::unordered_map<std::string, SDL_Texture*> textures;
};