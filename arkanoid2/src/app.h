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

	void AddBall(float startX, float startY, const std::string& textureID);
	void RemoveBall(uint16_t ballID);

	static float Speed;
	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static Logger* logger;
	static class AssetManager* assets;
	static uint32_t WINDOW_WIDTH;
	static uint32_t WINDOW_HEIGHT;
	static uint16_t ballsCount;
	static App* app;
	static SDL_Event event;
private:
	Player* player = nullptr;
	bool m_IsRunning = false;
	std::vector<Ball*> balls;
};