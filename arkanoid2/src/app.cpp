#include "app.h"
#include "assetManager.h"


#include <fstream>
#include <sstream>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

const uint32_t App::WINDOW_WIDTH = 800;
const uint32_t App::WINDOW_HEIGHT = 600;

const uint32_t App::s_AffectTime = 9000; // time in ms
const float App::s_MaxSpeedX = 2.0f;
const float App::s_MinSpeedY = 0.5f;
const float App::s_TilesWidth = (float)App::WINDOW_WIDTH / 20.0f;
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

	s_Window = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, App::WINDOW_WIDTH, App::WINDOW_HEIGHT, 0);
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
	s_Assets->LoadTexture("player", "assets/player64x32.png");
	s_Assets->LoadTexture("floorSquare", "assets/floor_square.png");

	s_Assets->LoadTexture(TextureOfTile(TileType::wall), "assets/tiles/wall_tile.png");
	s_Assets->LoadTexture(TextureOfTile(TileType::green), "assets/tiles/green_tile.png");
	s_Assets->LoadTexture(TextureOfTile(TileType::blue), "assets/tiles/blue_tile.png");

	// Perks
	s_Assets->LoadTexture(TextureOfPerk(PerkType::none), "assets/perks/none.png");
	s_Assets->LoadTexture(TextureOfPerk(PerkType::shrink), "assets/perks/shrink_player.png");
	s_Assets->LoadTexture(TextureOfPerk(PerkType::supersize), "assets/perks/supersize_player.png");
	s_Assets->LoadTexture(TextureOfPerk(PerkType::addball), "assets/perks/add_ball.png");
	s_Assets->LoadTexture(TextureOfPerk(PerkType::duplicateball), "assets/perks/duplicate_ball.png");
	s_Assets->LoadTexture(TextureOfPerk(PerkType::floor), "assets/perks/floor.png");
	s_Assets->LoadTexture("perkHidden", "assets/perks/hidden_perk.png");

	{
		std::string levelDir = "assets\\levels\\";
		std::string levelExtension = ".level";

		levels.reserve(amountOfLevels);

		for (uint16_t i = 0; i < amountOfLevels; i++)
		{
			std::ifstream levelFile(levelDir + std::to_string(i) + levelExtension);

			// struct Level is defined in app.h
			// Its only contain is std::vector<std::vector<TileType>> levelData and bool m_IsFailed

			//levels[i] = std::move(std::unique_ptr<Level>());
			levels.emplace_back(std::move(std::make_unique<Level>()));

			if (levelFile.fail())
			{
				s_Logger->Print(typeid(*this).name(), std::to_string(i) + levelExtension + " couldn't be find");
				levels.back().get()->m_IsFailed = true;
				continue;
			}

			std::string line;

			while (std::getline(levelFile, line)) {
				std::istringstream ss(line);
				std::vector<TileType> row;
				std::string value;

				while (std::getline(ss, value, ',')) {
					row.push_back((TileType)std::stoi(value));
				}

				levels.back().get()->levelData.push_back(row);
			}
		}
	}

	{
		Level* currentLevel = levels.at(currentLevelID).get();
		TileType tileType;

		for (uint32_t y = 0; y < 15; y++)
		{
			for (uint32_t x = 0; x < 20; x++)
			{
				tileType = currentLevel->levelData.at(y).at(x);

				if (!(uint32_t)tileType)
				{
					continue;
				}

				AddTile(tileType, (x * App::s_TilesWidth), (y * 16.0f));
			}
		}
	}

	//for (int i = 0; i < 100; ++i)
	AddBall((float)App::WINDOW_WIDTH / 2, ((float)App::WINDOW_HEIGHT / 2), 0.0f, 1.0f);

	player = s_Manager->NewEntity<Player>();

	floor = s_Manager->NewEntity<Floor>();

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

	s_Logger->Print(typeid(*this).name(), std::string("Levels: ") + std::to_string(levels.size()));
	s_Logger->Print(typeid(*this).name(), std::string("Balls: ") + std::to_string(balls.size()));
	s_Logger->Print(typeid(*this).name(), std::string("Tiles: ") + std::to_string(tiles.size()));
	s_Logger->Print(typeid(*this).name(), std::string("Perks: ") + std::to_string(perks.size()));
	s_Logger->Print(typeid(*App::s_Manager).name(), std::string("Entities: ") + std::to_string(App::s_Manager->GetEntitiesAmount()));

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
		{
			switch (App::s_Event.key.keysym.sym)
			{
				// display balls size in console
			case SDLK_TAB:
				App::s_Logger->Print(typeid(*this).name(), std::string("Active balls: ") + std::to_string(balls.size()));
				break;
				// drop perk duplicate ball
			case SDLK_F1:
				App::s_Manager->NewEntity<Perk>(TextureOfPerk(PerkType::duplicateball), player->GetPos().x + player->GetPos().w / 2, player->GetPos().y - 30.0f, PerkType::duplicateball);
				break;
				// drop perk add ball
			case SDLK_F2:
				App::s_Manager->NewEntity<Perk>(TextureOfPerk(PerkType::addball), player->GetPos().x + player->GetPos().w / 2, player->GetPos().y - 30.0f, PerkType::addball);
				break;
				// change invisible mode for player
			case SDLK_F3:
				player->SwitchInvisible();

				App::s_Logger->Print(typeid(*this).name(), std::to_string(player->GetInvisible()));
				break;
				// drop perk floor
			case SDLK_F4:
				App::s_Manager->NewEntity<Perk>(TextureOfPerk(PerkType::floor), player->GetPos().x + player->GetPos().w / 2, player->GetPos().y - 30.0f, PerkType::floor);
				break;
				// Load next level
			case SDLK_F5:
				currentLevelID++;
				LoadLevel();
				break;
				// Drop supersize perk
			case SDLK_F6:
				App::s_Manager->NewEntity<Perk>(TextureOfPerk(PerkType::supersize), player->GetPos().x + player->GetPos().w / 2, player->GetPos().y - 30.0f, PerkType::supersize);
				break;
				// spawn ball directly in the middle
			case SDLK_F10:
				AddBall(static_cast<float>(WINDOW_WIDTH / 2), static_cast<float>(WINDOW_HEIGHT / 2), 0.0f, 1.0f);
				break;
			default:
				break;
			}
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

		const SDL_FRect& ballPos = b->GetPos();

		// TODO: Fix calculations for bouncing ball and adjust SDL_HasIntersectionF to project's needs

		// Check for hitting the floor if it's been shown
		if (floor->IsShown() && SDL_HasIntersectionF(&ballPos, &floor->GetPos()))
		{
			static_cast<Ball*>(b)->HitPlayer(floor->GetPos());
			continue;
		}

		// Check for hitting a player
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

				if (static_cast<Tile*>(t)->GetType() == TileType::wall)
				{
					continue;
				}

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
			AddBall(playerPos.x + playerPos.w / 2, playerPos.y - Ball::s_OriginalHeight * 1.5f, 0.0f, 1.0f);
			break;
		case PerkType::duplicateball:
			{
				// just save place of an element instead of inserting element
				std::vector<std::size_t> temporaryVector;

				for (std::size_t i = 0; i < balls.size(); i++)
				{
					if (!balls.at(i)->IsActive())
					{
						continue;
					}

					temporaryVector.push_back(i);
				}

				for (const auto& i : temporaryVector)
				{
					const SDL_FRect& pos = balls.at(i)->GetPos();
					const Velocity& velocity = static_cast<Ball*>(balls.at(i))->GetVelocity();

					AddBall(pos.x + velocity.x, pos.y, -velocity.x, velocity.y);
				}
			}
			break;
		case PerkType::floor:
			if (floor->IsShown())
			{
				floor->Extend();
			}
			else
			{
				floor->Toggle();
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

	floor->Draw();

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

void App::AddBall(float startX, float startY, float velocityX, float velocityY)
{
	//if (balls.size() >= 10000) // limit for active balls
	//{
	//	s_Logger->Print(typeid(*this).name(), "Balls have reached their limit (" + std::to_string(balls.size()) + ")");
	//	return;
	//}

	s_Manager->NewEntity<Ball>(startX, startY, velocityX, velocityY);
}

void App::AddTile(TileType type, float posX, float posY)
{
	s_Manager->NewEntity<Tile>(TextureOfTile(type), type, posX, posY);
}

void App::DropPerk(float posX, float posY)
{
	static std::default_random_engine rng(rnd());
	static std::uniform_real_distribution<float> realDistr(0, 1);

	// Should be appropriate to the count of perk types
	static std::uniform_int_distribution<uint32_t> perkTypeDistr(1, (uint32_t)PerkType::size - 1);

	// probability that a perk drops among tiles
	constexpr float perkDropChance = 0.15f;
	// probability that a perk has a hidden texture
	constexpr float perkHideChance = 0.20f;

	bool showTexture = true;
	if (realDistr(rng) <= perkHideChance) {
		showTexture = false;
	}

	if (realDistr(rng) <= perkDropChance)
	{
		PerkType perkType = (PerkType)perkTypeDistr(rng);

		std::string_view textureID = showTexture ? TextureOfPerk(perkType) : "perkHidden";

		s_Manager->NewEntity<Perk>(textureID, posX, posY, perkType);
	}
}

void App::LoadLevel()
{
	if (currentLevelID < 0)
	{
		currentLevelID = 0;
	}

	if (currentLevelID >= amountOfLevels)
	{
		App::s_Logger->Print(typeid(*this).name(), std::string("Level ") + std::to_string(currentLevelID) + " doesn't exist");
		currentLevelID = -1;
		return;
	}

	if (!levels.at(currentLevelID) || levels.at(currentLevelID)->m_IsFailed)
	{
		App::s_Logger->Print(typeid(levels.at(currentLevelID)).name(), std::string("Couldn't load level ") + std::to_string(currentLevelID));
		currentLevelID = -1;
		return;
	}

	// Clean all stuff
	for (const auto& t : tiles)
	{
		t->Destroy();
	}

	for (const auto& b : balls)
	{
		b->Destroy();
	}

	for (const auto& p : perks)
	{
		p->Destroy();
	}

	player->CancelAffect();
	
	if (floor->IsShown())
	{
		floor->Toggle();
	}

	Level* currentLevel = levels.at(currentLevelID).get();
	TileType tileType;

	for (uint32_t y = 0; y < 15; y++)
	{
		for (uint32_t x = 0; x < 20; x++)
		{
			tileType = currentLevel->levelData.at(y).at(x);

			if (!(uint32_t)tileType)
			{
				continue;
			}

			AddTile(tileType, (x * App::s_TilesWidth), (y * 16.0f));
		}
	}

	AddBall((float)App::WINDOW_WIDTH / 2, (float)App::WINDOW_HEIGHT / 2, 0.0f, 1.0f);
}