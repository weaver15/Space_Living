#pragma once
#include <string>
#include <SDL.h>
#include<cmath>

class Calculate
{
public:
	Calculate();
	~Calculate();

	std::pair <double, double> CalculateVelocity(int movementDirectionX, int movementDirectionY, int speed);
	bool WithinDistance(SDL_Rect* p1, SDL_Rect* p2, float maxDistance);
	int Distance(SDL_Point* p1, SDL_Point* p2);
};

