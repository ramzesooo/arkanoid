#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <memory>

struct proxy_hash {
	using is_transparent = void;
	// abbreviated function template (C++20)
	std::size_t operator()(const auto& key) const {
		return std::hash<std::remove_cvref_t<decltype(key)>>{}(key);
	}
};

class AssetManager
{
public:
	~AssetManager();

	//void LoadTexture(const std::string& textureID, const std::string& path);
	void LoadTexture(std::string_view textureID, const std::string& path);
	void DrawTexture(std::string_view textureID, const SDL_Rect& src, const SDL_FRect& dest);
private:
	std::unordered_map<std::string, SDL_Texture*, proxy_hash, std::equal_to<void>> textures;
};