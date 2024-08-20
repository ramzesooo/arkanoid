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

const uint32_t App::WINDOW_WIDTH = 800;
const uint32_t App::WINDOW_HEIGHT = 600;

float App::s_MaxSpeedX = 2.0f;
float App::s_MinSpeedY = 0.5f;
float App::s_TilesWidth = (float)App::WINDOW_WIDTH / 16.0f;
SDL_Window* App::s_Window = nullptr;
SDL_Renderer* App::s_Renderer = nullptr;
Logger* App::s_Logger = nullptr;
AssetManager* App::s_Assets = nullptr;
SDL_Event App::s_Event;
std::unique_ptr<Manager> App::s_Manager = std::make_unique<Manager>();

App::App()
{
	s_Logger = new Logger();
	s_Assets = new AssetManager();

	// initialized is going to be changed if something is wrong (i.e. couldn't create a window)
	// running state of the game depends on this in the end of the constructor
	bool initialized = true;

	s_Window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!s_Window)
	{
		s_Logger->LogSDL("Creating window: ");
		initialized = false;
	}

	s_Renderer = SDL_CreateRenderer(s_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!s_Renderer)
	{
		s_Logger->LogSDL("Creating renderer: ");
		initialized = false;
	}
	
	SDL_Surface* iconSurface = IMG_Load("assets/gugu.png");
	SDL_SetWindowIcon(s_Window, iconSurface);
	SDL_FreeSurface(iconSurface);

	SDL_SetRenderDrawColor(s_Renderer, 100, 50, 200, 255);

	// Prepare all needed textures for the game and hold it in unordered_map inside AssetManager class
	// First string is ID of the texture necessary to draw it.
	s_Assets->LoadTexture("defaultBall", "assets/ball256x256.png");
	s_Assets->LoadTexture("greenTile", "assets/green_tile.png");
	s_Assets->LoadTexture("player", "assets/player64x32.png");

	// Just simple way to create some tiles, until it will be ready to create appropriate levels
	for (int y = 0; y < 10; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			AddTile("greenTile", (x * App::s_TilesWidth), (y * 16.0f));
		}
	}

	for (int i = 0; i < 10; ++i)
		AddBall((float)WINDOW_WIDTH / 2, ((float)WINDOW_HEIGHT / 2) + i, { 0.0f, 1.0f });

	player = s_Manager->NewEntity<Player>();
	player->AddGroup(groupPlayers);

	m_IsRunning = initialized;

	s_Logger->LogConstructor(typeid(*this).name());
}

auto& balls = App::s_Manager->GetGroup(App::groupBalls);
auto& players = App::s_Manager->GetGroup(App::groupPlayers);
auto& tiles = App::s_Manager->GetGroup(App::groupTiles);

App::~App()
{
	delete App::s_Assets;

	SDL_DestroyRenderer(s_Renderer);
	SDL_DestroyWindow(s_Window);
	SDL_Quit();

	s_Logger->LogDestructor(typeid(*this).name());
	delete App::s_Logger;
}

void App::EventHandler()
{
	SDL_PollEvent(&s_Event);

	switch (s_Event.type)
	{
	case SDL_QUIT:
		m_IsRunning = false;
		break;
	case SDL_KEYDOWN: // for easier testing, to be removed when it will be unncessary anymore
		if (s_Event.key.keysym.sym == SDLK_F10)
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
	// Refresh balls vector and manage all existing balls
	//for (auto it = balls.begin(); it != balls.end();)
	//{
	//	if (*it && (*it)->IsActive())
	//	{
	//		const SDL_FRect& ballPos = (*it)->GetPos();

	//		// Make the ball bounce after hitting a player
	//		if (CheckCollisions(ballPos, playerPos))
	//		{
	//			Velocity& velocity = (*it)->GetVelocity();

	//			float centerX = playerPos.x + playerPos.w / 2;
	//			float centerY = playerPos.y + playerPos.h / 2;

	//			// distance from the center of player
	//			float distanceX = ((ballPos.x + ballPos.w / 2) - centerX) / (playerPos.w / 2);

	//			velocity.x = distanceX * App::s_MaxSpeedX;

	//			velocity.y = -std::abs(velocity.y);

	//			if (std::abs(velocity.y) < App::s_MinSpeedY)
	//			{
	//				velocity.y = -App::s_MinSpeedY;
	//			}

	//			App::s_Logger->Print(typeid(*this).name(), std::to_string(velocity.x) + ", " + std::to_string(velocity.y));
	//		}

	//		it++;
	//	}
	//	else
	//	{
	//		it = balls.erase(it);
	//	}
	//}
	// End of refreshing balls vector

	// Refresh tiles vector and manage all existing tiles
	/*for (auto it = tiles.begin(); it != tiles.end();)
	{
		if (*it && (*it)->IsActive())
		{
			it++;
		}
		else
		{
			it = tiles.erase(it);
		}
	}*/
	// End of refreshing tiles vector

	s_Manager->Refresh();

	const SDL_FRect& playerPos = player->GetPos();

	for (const auto& b : balls)
	{
		const SDL_FRect& ballPos = b->GetPos();

		// TODO: Fix calculations for bouncing ball and adjust SDL_HasIntersectionF to project's needs

		// Check for hitting a player at first place
		if (SDL_HasIntersectionF(&ballPos, &playerPos))
		{
			// If the ball collides with a player, then trigger function HitPlayer() in Ball class and skip rest of the code
			static_cast<Ball*>(b)->HitPlayer(playerPos);
			continue;
		}

		// Check collisions with tiles
		for (const auto& t : tiles)
		{
			const SDL_FRect& tilePos = t->GetPos();

			if (SDL_HasIntersectionF(&ballPos, &tilePos))
			{
				// Make the ball bouncing
				static_cast<Ball*>(b)->HitTile(tilePos);

				// Logic for hitted tile
				// NOTE: Temporarily it's just destroying the tile
				// TODO: Let's make some perks dropping from tiles in future
				t->Destroy();

				//App::s_Logger->Print(typeid(*this).name(), "Tile has been hitted");
			}
		}
	}
	s_Manager->Update();
}

void App::Render()
{
	SDL_RenderClear(s_Renderer);

	//manager.Draw();
	for (const auto& t : tiles)
	{
		t->Draw();
	}

	for (const auto& b : balls)
	{
		b->Draw();
	}

	for (const auto& p : players)
	{
		p->Draw();
	}

	SDL_RenderPresent(s_Renderer);
}

void App::AddBall(float startX, float startY, Velocity velocity)
{
	if (balls.size() >= 15000) // limit for active balls
	{
		s_Logger->Print(typeid(*this).name(), "Balls have reached their limit (" + std::to_string(balls.size()) + ")");
		return;
	}

	auto* ball = s_Manager->NewEntity<Ball>(startX, startY, velocity);
	ball->AddGroup(groupBalls);
	//balls.push_back(ball);
}

void App::AddTile(const std::string& textureID, float posX, float posY)
{
	auto* tile = s_Manager->NewEntity<Tile>(textureID, posX, posY);
	tile->AddGroup(groupTiles);
	//tiles.push_back(tile);
}

bool App::CheckCollisions(const SDL_FRect& ballPos, const SDL_FRect& entityPos)
{
	return (ballPos.y + ballPos.h / 2 >= entityPos.y
		&& ballPos.y <= entityPos.y + entityPos.h
		&& ballPos.x + ballPos.w / 2 >= entityPos.x
		&& ballPos.x <= entityPos.x + entityPos.w);
}