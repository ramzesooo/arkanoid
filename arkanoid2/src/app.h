#pragma once
#include "entity.h"
#include "player.h"
#include "tile.h"
#include "ball.h"
#include "perk.h"
#include "log.h"

#include "SDL.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <random>

constexpr uint16_t amountOfLevels = 2;

struct Level
{
	std::vector<std::vector<TileType>> levelData;
	bool m_IsFailed = false;
};

class App
{
public:
	App();
	~App();

	bool IsRunning() const { return m_IsRunning; }

	// Functions called in main loop
	void EventHandler();
	void Update();
	void Render();
	// End



	void AddBall(float startX, float startY, float velocityX, float velocityY);
	void AddTile(TileType type, float posX, float posY);

	// DropPerk() is responsible for whole logic of checking the luck and drawing the perk
	void DropPerk(float posX, float posY);

	inline std::string_view TextureOfPerk(PerkType type)
	{
		using enum PerkType;

		switch (type)
		{
			case shrink: return "perkShrink";
			case supersize: return "perkSupersize";
			case addball: return "perkAddBall";
			case duplicateball: return "perkDuplicateBall";
		}

		return "perkNone";
	}

	inline std::string_view TextureOfTile(TileType type)
	{
		using enum TileType;

		switch (type)
		{
			case wall: return "wallTile";
			case green: return "greenTile";
			case blue: return "blueTile";
		}

		return "perkNone";
	}

	static const uint32_t WINDOW_WIDTH;
	static const uint32_t WINDOW_HEIGHT;

	static const uint32_t s_AffectTime; // time in ms
	static const float s_MaxSpeedX;
	static const float s_MinSpeedY;
	static const float s_TilesWidth;
	static SDL_Window* s_Window;
	static SDL_Renderer* s_Renderer;
	static Logger* s_Logger;
	static class AssetManager* s_Assets;
	static SDL_Event s_Event;
	static std::unique_ptr<Manager> s_Manager;
private:
	Player* player = nullptr;
	bool m_IsRunning = false;
	std::random_device rnd;

	std::size_t currentLevelID = 0;
	std::vector<std::unique_ptr<Level>> levels;
};