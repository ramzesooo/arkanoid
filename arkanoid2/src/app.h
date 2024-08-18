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

	bool IsRunning() const;

	void EventHandler();
	void Update();
	void Render();

	void AddBall(float startX, float startY, Velocity velocity);

	void AddTile(const std::string& textureID, float posX, float posY);

	bool CheckCollisions(const SDL_FRect& ballPos, const SDL_FRect& entityPos);

	static float MaxSpeedX;
	static float MinSpeedY;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static Logger* logger;
	static class AssetManager* assets;
	static const uint32_t WINDOW_WIDTH;
	static const uint32_t WINDOW_HEIGHT;
	static SDL_Event event;
private:
	Player* player = nullptr;
	bool m_IsRunning = false;
	std::vector<Ball*> balls;
	std::vector<Tile*> tiles;
};