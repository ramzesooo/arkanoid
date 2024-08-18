#pragma once
#include "SDL.h"
#include <vector>
#include <string>

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

	bool GetAppState() const;

	void EventHandler();
	void Update();
	void Render();

	void AddBall(float startX, float startY, const std::string& textureID, Velocity velocity);
	void RemoveBall(uint16_t ballID);

	void AddTile(const std::string& textureID, uint32_t posX, uint32_t posY);
	void RemoveTile(uint16_t tileID);

	static float MaxSpeedX;
	static float MinSpeedY;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static Logger* logger;
	static class AssetManager* assets;
	static uint32_t WINDOW_WIDTH;
	static uint32_t WINDOW_HEIGHT;
	static uint16_t ballsCount;
	static uint16_t tilesCount;
	static App* app;
	static SDL_Event event;
private:
	Player* player = nullptr;
	bool m_IsRunning = false;
	std::vector<Ball*> balls;
	std::vector<Tile*> tiles;
};