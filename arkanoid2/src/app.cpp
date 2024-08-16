#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "log.h"
#include "entity.h"
#include "player.h"
#include "ball.h"
#include "assetManager.h"
#include "app.h"

Manager manager;

SDL_Window* App::window = nullptr;
SDL_Renderer* App::renderer = nullptr;
Logger* App::logger = nullptr;
AssetManager* App::assets = nullptr;

uint32_t App::WINDOW_WIDTH = 800;
uint32_t App::WINDOW_HEIGHT = 600;

uint16_t App::ballsCount = 1;

App::App()
{
	logger = new Logger();
	assets = new AssetManager();

	SDL_PollEvent(&m_Event);
	bool initialized = true;

	window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!window)
	{
		logger->LogSDL("Creating window: ");
		initialized = false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		logger->LogSDL("Creating renderer: ");
		initialized = false;
	}

	SDL_SetRenderDrawColor(renderer, 100, 50, 200, 255);

	assets->LoadTexture("defaultBall", "assets/ball256x256.png");
	assets->LoadTexture("greenTile", "assets/green_tile.png");
	assets->LoadTexture("player", "assets/player64x32.png");

	AddBall(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, "defaultBall");

	player = new Player();
	manager.AddEntity(player);

	m_IsRunning = initialized;

	logger->LogConstructor(typeid(this).name());
}

App::~App()
{
	player->Destroy();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	delete App::assets;
	delete App::logger;

	logger->LogDeconstructor(typeid(this).name());
}

bool App::GetAppState() const
{
	return m_IsRunning;
}

void App::EventHandler()
{
	SDL_PollEvent(&m_Event);

	switch (m_Event.type)
	{
	case SDL_QUIT:
		m_IsRunning = false;
		break;
	default:
		break;
	}
}

void App::Update()
{
	manager.Refresh();
	manager.Update();
}

void App::Render()
{
	SDL_RenderClear(renderer);

	manager.Draw();

	SDL_RenderPresent(renderer);
}

void App::AddBall(uint32_t startX, uint32_t startY, const std::string& textureID)
{
	if (ballsCount >= 10000) // limit for active balls
	{
		logger->Print(typeid(this).name(), "Balls reached the limit.");
		return;
	}

	ballsCount++;
	Ball* ball = new Ball(startX, startY, textureID, ballsCount);

	manager.AddEntity(ball);
}

void App::RemoveBall(uint16_t ballID)
{
	for (auto it = balls.begin(); it != balls.end(); it++)
	{
		if ((*it)->GetBallID() != ballID)
		{
			continue;
		}

		(*it)->Destroy();
		balls.erase(it);
		ballsCount--;
		break;
	}
}