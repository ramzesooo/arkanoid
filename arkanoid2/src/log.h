#pragma once
#include <string>

class Ball;
class Entity;
class Manager;
class App;

class Logger
{
public:
	void LogSDL(const std::string& additionalText) const;
	void LogTTF(const std::string& additionalText) const;

	void LogConstructor(const std::string& tag) const;
	void LogDestructor(const std::string& tag) const;

	void Debug(const std::string& tag, const std::string& text) const;
	void Print(const std::string& tag, const std::string& text) const;
};