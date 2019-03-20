#pragma once
#include <string>

class ErrorManager
{
public:
	ErrorManager();
	~ErrorManager();

	void LogError(std::string tag, std::string location, std::string message);
};

