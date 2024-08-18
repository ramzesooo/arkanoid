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
SDL_Event App::event;

const uint32_t App::WINDOW_WIDTH = 800;
const uint32_t App::WINDOW_HEIGHT = 600;

App::App()
{
	logger = new Logger();
	assets = new AssetManager();

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

	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			AddTile("greenTile", (x * 64.0f), (y * 16.0f));
		}
	}

	AddBall(static_cast<float>(WINDOW_WIDTH / 2), static_cast<float>(WINDOW_HEIGHT / 2), { 0.0f, 1.0f });
	player = static_cast<Player*>(manager.NewEntity<Player>());

	m_IsRunning = initialized;

	logger->LogConstructor(typeid(*this).name());
}

App::~App()
{
	delete App::assets;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	logger->LogDeconstructor(typeid(*this).name());
	delete App::logger;
}

bool App::IsRunning() const
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
			AddBall(static_cast<float>(WINDOW_WIDTH / 2), static_cast<float>(WINDOW_HEIGHT / 2), { 0.0f, 1.0f } );
		}
		break;
	default:
		break;
	}
}

void App::Update()
{
	const SDL_FRect& playerPos = player->GetPos();

	for (const auto& ball : balls)
	{
		const SDL_FRect& ballPos = ball->GetPos();

		if (CheckCollisions(ballPos, playerPos))
		{
			Velocity& velocity = ball->GetVelocity();

			float centerX = static_cast<float>(playerPos.x + playerPos.w / 2);
			float centerY = static_cast<float>(playerPos.y + playerPos.h / 2);

			float distanceX = ((ballPos.x + ballPos.w / 2) - centerX) / (static_cast<float>(playerPos.w) / 2);

			velocity.x = distanceX * App::MaxSpeedX;

			velocity.y = -std::abs(velocity.y);

			if (std::abs(velocity.y) < App::MinSpeedY)
			{
				velocity.y = -App::MinSpeedY;
			}

			App::logger->Print(typeid(*this).name(), std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
		}
	}

	manager.Refresh();
	manager.Update();
}

void App::Render()
{
	SDL_RenderClear(renderer);

	manager.Draw();

	SDL_RenderPresent(renderer);
}

void App::AddBall(float startX, float startY, Velocity velocity)
{
	if (balls.size() >= UINT16_MAX / 2) // limit for active balls
	{
		logger->Print(typeid(*this).name(), "Balls reached the limit (" + std::to_string(balls.size()) + ")");
		return;
	}

	auto* ball = static_cast<Ball*>(manager.NewEntity<Ball>(startX, startY, velocity));
	balls.push_back(ball);
}

//void App::AddBall(float startX, float startY, const std::string& textureID, Velocity velocity)
//{
//	if (balls.size() >= UINT16_MAX / 2) // limit for active balls
//	{
//		logger->Print(typeid(*this).name(), "Balls reached the limit (" + std::to_string(balls.size()) + ")");
//		return;
//	}
//
//	Ball* ball = new Ball(startX, startY, textureID, velocity);
//
//	manager.AddEntity(ball);
//	balls.push_back(ball);
//}

void App::AddTile(const std::string& textureID, float posX, float posY)
{
	auto* tile = static_cast<Tile*>(manager.NewEntity<Tile>(textureID, posX, posY));
	tiles.push_back(tile);
}

bool App::CheckCollisions(const SDL_FRect& ballPos, const SDL_FRect& entityPos)
{
	if (ballPos.y + ballPos.h / 2 >= entityPos.y
		&& ballPos.y <= entityPos.y + entityPos.h
		&& ballPos.x + ballPos.w / 2 >= entityPos.x
		&& ballPos.x <= entityPos.x + entityPos.w)
	{
		return true;
	}

	return false;
}