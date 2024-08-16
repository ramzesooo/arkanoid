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

	assets->LoadTexture("ball", "assets/ball256x256.png");
	assets->LoadTexture("greenTile", "assets/green_tile.png");
	assets->LoadTexture("player", "assets/player64x32.png");

	AddBall();
	AddPlayer();

	m_IsRunning = initialized;

	logger->LogConstructor(typeid(this).name());
}

App::~App()
{
	logger->LogDeconstructor(typeid(this).name());

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	delete App::assets;
	delete App::logger;
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

void App::AddPlayer()
{
	Player* player = new Player();

	manager.AddEntity(player);
}

void App::AddBall()
{
	Ball* ball = new Ball();

	manager.AddEntity(ball);
}

void App::RemoveBall()
{

}