#pragma once
#include "SDL.h"
#include <vector>
#include <string>
#include <memory>

class Logger;
class Entity;
class Ball;
class Tile;
class Player;
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

	static const uint32_t WINDOW_WIDTH;
	static const uint32_t WINDOW_HEIGHT;

	static float s_MaxSpeedX;
	static float s_MinSpeedY;
	static float s_TilesWidth;
	static SDL_Window* s_Window;
	static SDL_Renderer* s_Renderer;
	static Logger* s_Logger;
	static class AssetManager* s_Assets;
	static SDL_Event s_Event;
private:
	Player* player = nullptr;
	bool m_IsRunning = false;
	std::vector<Ball*> balls;
	std::vector<Tile*> tiles;
};