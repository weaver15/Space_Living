#include "Calculate.h"



Calculate::Calculate()
{
}


Calculate::~Calculate()
{
}

std::pair <double, double> Calculate::CalculateVelocity(int movementDirectionX, int movementDirectionY, int speed)
{
	double factor = speed /
		sqrt((movementDirectionX * movementDirectionX) + (movementDirectionY * movementDirectionY));
	return std::pair <double, double> { movementDirectionX * factor, movementDirectionY * factor };
}

int Calculate::Distance(SDL_Point* p1, SDL_Point* p2)
{
	//This isn't the actual distance, that would need to be sqrt but I don't need that at the moment
	return ((p2->x - p1->x) * (p2->x - p1->x) + (p2->y - p1->y) * (p2->y - p1->y));
}

bool Calculate::WithinDistance(SDL_Rect* p1, SDL_Rect* p2, float maxDistance)
{
	return (pow((p2->x - p1->x), 2) + pow((p2->y - p1->y), 2)) < pow(maxDistance, 2);
}