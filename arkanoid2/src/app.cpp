#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "log.h"
#include "entity.h"
#include "player.h"
#include "ball.h"
#include "tile.h"
#include "assetManager.h"
#include "app.h"

Manager manager;

float App::MaxSpeedX = 2.0f;
float App::MinSpeedY = 0.5f;
SDL_Window* App::window = nullptr;
SDL_Renderer* App::renderer = nullptr;
Logger* App::logger = nullptr;
AssetManager* App::assets = nullptr;
App* App::app = nullptr;
SDL_Event App::event;

uint32_t App::WINDOW_WIDTH = 800;
uint32_t App::WINDOW_HEIGHT = 600;

uint16_t App::ballsCount = 1;
uint16_t App::tilesCount = 0;

App::App()
{
	logger = new Logger();
	assets = new AssetManager();
	app = this;

	SDL_PollEvent(&event);
	bool initialized = true;

	window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!window)
	{
		logger->LogSDL("Creating window: ");
		initialized = false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		logger->LogSDL("Creating renderer: ");
		initialized = false;
	}
	
	SDL_Surface* iconSurface = IMG_Load("assets/gugu.png");
	SDL_SetWindowIcon(window, iconSurface);
	SDL_FreeSurface(iconSurface);

	SDL_SetRenderDrawColor(renderer, 100, 50, 200, 255);

	assets->LoadTexture("defaultBall", "assets/ball256x256.png");
	assets->LoadTexture("greenTile", "assets/green_tile.png");
	assets->LoadTexture("player", "assets/player64x32.png");

	AddBall(static_cast<float>(WINDOW_WIDTH / 2), static_cast<float>(WINDOW_HEIGHT / 2), "defaultBall", { 0.0f, 1.0f });

	player = new Player();
	manager.AddEntity(player);

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			AddTile("greenTile", (x * 64), (y * 16));
		}
	}

	m_IsRunning = initialized;

	logger->LogConstructor(typeid(*this).name());
}

App::~App()
{
	player->Destroy();
	delete App::assets;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	logger->LogDeconstructor(typeid(*this).name());
	delete App::logger;
}

bool App::GetAppState() const
{
	return m_IsRunning;
}

void App::EventHandler()
{
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		m_IsRunning = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_F10)
		{
			AddBall(static_cast<float>(WINDOW_WIDTH / 2), static_cast<float>(WINDOW_HEIGHT / 2), "defaultBall", { 0.0f, 1.0f } );
		}
		break;
	default:
		break;
	}
}

void App::Update()
{
	manager.Refresh();
	manager.Update();

	for (const auto& ball : balls)
	{
		ball->AABB(player->GetPos());
	}
}

void App::Render()
{
	SDL_RenderClear(renderer);

	manager.Draw();

	SDL_RenderPresent(renderer);
}

void App::AddBall(float startX, float startY, const std::string& textureID, Velocity velocity)
{
	if (ballsCount >= 16500) // limit for active balls
	{
		logger->Print(typeid(*this).name(), "Balls reached the limit (" + std::to_string(ballsCount) + ")");
		return;
	}

	Ball* ball = new Ball(startX, startY, textureID, ballsCount, velocity);

	manager.AddEntity(ball);
	balls.push_back(ball);
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
		break;
	}
}

void App::AddTile(const std::string& textureID, uint32_t posX, uint32_t posY)
{
	Tile* tile = new Tile(textureID, posX, posY);

	manager.AddEntity(tile);
	tiles.push_back(tile);
}

void App::RemoveTile(uint16_t tileID)
{
	for (auto it = tiles.begin(); it != tiles.end(); it++)
	{
		if ((*it)->GetTileID() != tileID)
		{
			continue;
		}

		(*it)->Destroy();
		tiles.erase(it);
		break;
	}
}