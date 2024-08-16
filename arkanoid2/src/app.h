#pragma once
#include "SDL.h"
#include <vector>
#include <string>

class Logger;
class Entity;
class Ball;
class Player;

class App
{
public:
	App();
	~App();

	bool GetAppState() const;

	void EventHandler();
	void Update();
	void Render();

	void AddBall(uint32_t startX, uint32_t startY, const std::string& textureID);
	void RemoveBall(uint16_t ballID);

	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static Logger* logger;
	static class AssetManager* assets;
	static uint32_t WINDOW_WIDTH;
	static uint32_t WINDOW_HEIGHT;
	static uint16_t ballsCount;
private:
	Player* player = nullptr;
	SDL_Event m_Event;
	bool m_IsRunning = false;
	std::vector<Ball*> balls;
};