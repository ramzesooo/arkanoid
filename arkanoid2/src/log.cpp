#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "log.h"

void Logger::LogSDL(const std::string& additionalText = "") const
{
	std::cout << "\n" << additionalText;

	std::string gotError = SDL_GetError();

	if (gotError.size() > 0)
	{
		std::cout << gotError << std::endl;
		SDL_ClearError();
	}
	else
	{
		std::cout << "No SDL errors found" << std::endl;
	}
}

void Logger::LogTTF(const std::string& additionalText = "") const
{
	std::cout << "\n" << additionalText;

	std::string gotError = TTF_GetError();

	if (gotError.size() > 0)
	{
		std::cout << gotError << std::endl;
		SDL_ClearError();
	}
	else
	{
		std::cout << "No TTF errors found" << std::endl;
	}
}

void Logger::LogConstructor(const std::string& tag) const
{
	std::cout << tag << " has been created" << std::endl;
}

void Logger::LogDestructor(const std::string& tag) const
{
	std::cout << tag << " has been destroyed" << std::endl;
}

void Logger::Debug(const std::string& tag, const std::string& text) const
{
	std::cout << tag << "::" << text << "\n";
}

void Logger::Print(const std::string& tag, const std::string& text) const
{
	std::cout << tag << ": " << text << "\n";
}