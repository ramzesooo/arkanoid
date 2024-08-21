#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <memory>
#include <random>

class Logger;
class Entity;
class Ball;
class Tile;
class Player;
class Manager;

struct Velocity;

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

	void AddBall(float startX, float startY, Velocity velocity);

	void AddTile(const std::string& textureID, float posX, float posY);

	bool CheckCollisions(const SDL_FRect& ballPos, const SDL_FRect& entityPos);

	// DropPerk() is responsible for the whole logic of checking the luck and drawing the perk
	void DropPerk(float posX, float posY);

	enum entityGroups : std::size_t
	{
		groupBalls = 0,
		groupPlayers,
		groupTiles,
		groupPerks
	};

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
};