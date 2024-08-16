#pragma once
#include <string>

class Ball;
class Entity;
class Manager;
class App;

class Logger
{
public:
	void LogSDL(const std::string& additionalText);
	void LogTTF(const std::string& additionalText);

	void LogConstructor(const std::string& tag);
	void LogDeconstructor(const std::string& tag);

	void Debug(const std::string& tag, const std::string& text);
	void Print(const std::string& tag, const std::string& text);
};