#pragma once
#include "SDL.h"

class Logger;
class Entity;
class Ball;

class App
{
public:
	App();
	~App();

	bool GetAppState() const;

	void EventHandler();
	void Update();
	void Render();

	void AddPlayer();

	void AddBall();
	void RemoveBall();

	static SDL_Window* window;
	static SDL_Renderer* renderer;
	static Logger* logger;
	static class AssetManager* assets;
private:
	SDL_Event m_Event;
	bool m_IsRunning = false;
};