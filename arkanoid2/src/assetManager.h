#pragma once
#include <unordered_map>
#include <string>

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	void LoadTexture(const std::string& textureID, const std::string& path);
	void DrawTexture(const std::string& textureID);
private:
	std::unordered_map<std::string, SDL_Texture*> textures;
};