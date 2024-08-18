#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "log.h"
#include "app.h"

int main(int argc, char** arg)
{
	{
		Logger logger;

		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			logger.LogSDL("SDL_Init: ");
			return -1;
		}

		if (TTF_Init() != 0)
		{
			logger.LogTTF("TTF_Init: ");
			return -1;
		}
	}

	App app;

	while (app.IsRunning())
	{
		app.EventHandler();
		app.Update();
		app.Render();
	}

	return 0;
}