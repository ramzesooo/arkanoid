#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "assetManager.h"
#include "app.h"

const uint32_t App::WINDOW_WIDTH = 800;
const uint32_t App::WINDOW_HEIGHT = 600;

const uint32_t App::s_AffectTime = 15000; // time in ms
const float App::s_MaxSpeedX = 2.0f;
const float App::s_MinSpeedY = 0.5f;
const float App::s_TilesWidth = (float)App::WINDOW_WIDTH / 16.0f;
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

	// Primary textures
	s_Assets->LoadTexture("defaultBall", "assets/ball256x256.png");
	s_Assets->LoadTexture("greenTile", "assets/green_tile.png");
	s_Assets->LoadTexture("player", "assets/player64x32.png");

	// Perks
	s_Assets->LoadTexture("perkNone", "assets/perks/none.png");
	s_Assets->LoadTexture("perkShrink", "assets/perks/shrink_player.png");
	s_Assets->LoadTexture("perkSupersize", "assets/perks/supersize_player.png");
	s_Assets->LoadTexture("perkAddBall", "assets/perks/add_ball.png");
	s_Assets->LoadTexture("perkDuplicateBall", "assets/perks/duplicate_ball.png");
	s_Assets->LoadTexture("perkUnknown", "assets/perks/unknown_perk.png");

	// Just simple way to create some tiles, until it will be ready to create appropriate levels
	for (int y = 0; y < 14; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			AddTile("greenTile", (x * App::s_TilesWidth), (y * 16.0f));
		}
	}

	for (int i = 0; i < 100; ++i)
		AddBall((float)WINDOW_WIDTH / 2, ((float)WINDOW_HEIGHT / 2) + (0.5f * i), {0.0f, 1.0f});

	player = s_Manager->NewEntity<Player>();
	//player->AddGroup(EntityGroup::players);

	m_IsRunning = initialized;

	s_Logger->LogConstructor(typeid(*this).name());
}

auto& balls = App::s_Manager->GetGroup(EntityGroup::balls);
auto& players = App::s_Manager->GetGroup(EntityGroup::players);
auto& tiles = App::s_Manager->GetGroup(EntityGroup::tiles);
auto& perks = App::s_Manager->GetGroup(EntityGroup::perks);

App::~App()
{
	delete App::s_Assets;

	App::s_Logger->Print(typeid(*this).name(), std::string("Balls: ") + std::to_string(balls.size()));
	App::s_Logger->Print(typeid(*this).name(), std::string("Tiles: ") + std::to_string(tiles.size()));
	App::s_Logger->Print(typeid(*this).name(), std::string("Perks: ") + std::to_string(perks.size()));
	App::s_Logger->Print(typeid(*App::s_Manager).name(), std::string("Entities: ") + std::to_string(App::s_Manager->GetEntitiesAmount()));

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
		else if (s_Event.key.keysym.sym == SDLK_F1)
		{
			std::string_view textureID = textureOf(PerkType::duplicateball);

			s_Manager->NewEntity<Perk>(textureID, player->GetPos().x + player->GetPos().w / 2, player->GetPos().y - 50, PerkType::duplicateball);
		}
		else if (s_Event.key.keysym.sym == SDLK_F2)
		{
			std::string_view textureID = textureOf(PerkType::addball);

			s_Manager->NewEntity<Perk>(textureID, player->GetPos().x + player->GetPos().w / 2, player->GetPos().y - 50, PerkType::addball);
		}
		else if (s_Event.key.keysym.sym == SDLK_F3)
		{
			player->SetInvisible();

			App::s_Logger->Print(typeid(*this).name(), std::to_string(player->GetInvisible()));
		}
		break;
	default:
		break;
	}
}

void App::Update()
{
	const SDL_FRect& playerPos = player->GetPos();

	// Handling balls
	for (const auto& b : balls)
	{
		if (!b->IsActive())
		{
			continue;
		}

		if (static_cast<Ball*>(b)->GetVelocity().y == 0.0f)
		{
			b->Destroy();
			continue;
		}

		const SDL_FRect& ballPos = b->GetPos();

		// TODO: Fix calculations for bouncing ball and adjust SDL_HasIntersectionF to project's needs

		// Check for hitting a player at first place
		if (!player->GetInvisible() && SDL_HasIntersectionF(&ballPos, &playerPos))
		{
			// If the ball collides with a player, then trigger function HitPlayer() in Ball class and skip rest of the code
			static_cast<Ball*>(b)->HitPlayer(playerPos);
			continue;
		}

		// Check collisions with tiles
		for (const auto& t : tiles)
		{
			if (!t->IsActive())
			{
				continue;
			}

			const SDL_FRect& tilePos = t->GetPos();

			if (SDL_HasIntersectionF(&ballPos, &tilePos))
			{
				// Make the ball bouncing
				static_cast<Ball*>(b)->HitTile(tilePos);

				DropPerk(tilePos.x, tilePos.y);
				t->Destroy();
			}
		}
	}

	// Handling perks
	for (const auto& perk : perks)
	{
		if (!perk->IsActive() || !SDL_HasIntersectionF(&perk->GetPos(), &player->GetPos()))
		{
			continue;
		}

		PerkType perkType = static_cast<Perk*>(perk)->GetType();

		perk->Destroy();

		if (perkType == PerkType::none)
		{
			continue;
		}

		switch (perkType)
		{
		case PerkType::shrink:
		case PerkType::supersize:
			player->SetAffect(perkType);
			break;
		case PerkType::addball:
			{
				for (auto it = balls.rbegin(); it != balls.rend(); it++)
				{
					if (!(*it)->IsActive())
					{
						continue;
					}

					SDL_FRect pos = (*it)->GetPos();
					Velocity velocity = static_cast<Ball*>(*it)->GetVelocity();
					velocity.x = -velocity.x;

					if (velocity.y > 0.0f)
					{
						velocity.y = -velocity.y;
					}

					AddBall(pos.x + velocity.x, pos.y, velocity);
					break;
				}
			}
			break;
		case PerkType::duplicateball:
			{
				// just save place of an element instead of inserting element
				std::vector<std::size_t> temporaryVector;

				for (std::size_t i = 0; i < balls.size(); i++)
				{
					if (!balls[i]->IsActive())
					{
						continue;
					}

					temporaryVector.push_back(i);
				}

				for (const auto& ball : temporaryVector)
				{
					const SDL_FRect pos = balls[ball]->GetPos();
					Velocity velocity = static_cast<Ball*>(balls[ball])->GetVelocity();
					velocity.x = -velocity.x;

					if (velocity.y > 0.0f)
					{
						velocity.y = -velocity.y;
					}

					AddBall(pos.x + velocity.x, pos.y, velocity);
				}

				/*std::vector<Entity*> temporaryVector;

				for (const auto& ball : balls)
				{
					if (!ball->IsActive())
					{
						continue;
					}

					temporaryVector.push_back(ball);
				}

				balls.reserve(temporaryVector.size());

				for (const auto& newBall : temporaryVector)
				{
					const SDL_FRect pos = newBall->GetPos();
					Velocity velocity = static_cast<Ball*>(newBall)->GetVelocity();
					velocity.x = -velocity.x;

					if (velocity.y > 0.0f)
					{
						velocity.y = -velocity.y;
					}

					AddBall(pos.x + velocity.x, pos.y, velocity);
				}

				temporaryVector.clear();*/
			}
			break;
		default:
			break;
		}
	}

	s_Manager->Refresh();
	s_Manager->Update();
}

void App::Render()
{
	SDL_RenderClear(s_Renderer);

	for (const auto& b : balls)
	{
		b->Draw();
	}

	for (const auto& t : tiles)
	{
		t->Draw();
	}

	for (const auto& perk : perks)
	{
		perk->Draw();
	}

	for (const auto& p : players)
	{
		p->Draw();
	}

	SDL_RenderPresent(s_Renderer);
}

void App::AddBall(float startX, float startY, Velocity velocity)
{
	//if (balls.size() >= 10000) // limit for active balls
	//{
	//	s_Logger->Print(typeid(*this).name(), "Balls have reached their limit (" + std::to_string(balls.size()) + ")");
	//	return;
	//}

	s_Manager->NewEntity<Ball>(startX, startY, velocity);

	//auto* ball = s_Manager->NewEntity<Ball>(startX, startY, velocity);
	//ball->AddGroup(EntityGroup::balls);
}

void App::AddTile(std::string_view textureID, float posX, float posY)
{
	s_Manager->NewEntity<Tile>(textureID, posX, posY);

	//auto* tile = s_Manager->NewEntity<Tile>(textureID, posX, posY);
	//tile->AddGroup(EntityGroup::tiles);
}

void App::DropPerk(float posX, float posY)
{
	static std::default_random_engine rng(rnd());
	static std::uniform_real_distribution<float> realDistr(0, 1);

	// Should be appropriate to the count of perk types
	static std::uniform_int_distribution<uint32_t> perkTypeDistr(1, (uint32_t)PerkType::size - 1);

	// probability that a perk drops among tiles
	constexpr float perkDropChance = 0.15f;
	// probability that a perk has a visible texture
	constexpr float perkShowChance = 0.8f;

	bool showTexture = false;
	if (realDistr(rng) <= perkShowChance) {
		showTexture = true;
	}

	if (realDistr(rng) <= perkDropChance)
	{
		PerkType perkType = (PerkType)perkTypeDistr(rng);

		std::string_view textureID = showTexture ? textureOf(perkType) : "perkUnknown";

		s_Manager->NewEntity<Perk>(textureID, posX, posY, perkType);

		//auto* perk = s_Manager->NewEntity<Perk>(textureID, posX, posY, perkType);
		//perk->AddGroup(EntityGroup::perks);
	}
}