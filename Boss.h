#pragma once
#include "Character.h"

class Boss : public Character
{
public: 
	Boss();
	Boss(std::pair<SDL_Rect, SDL_Rect> details);
	~Boss();

	void Update(SDL_Rect* playerLocation, ProjectileManager* projectiles, std::vector<SDL_Rect> solids, SDL_Rect * camera);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera); 
	void Activate();
private:
	bool IsActive; 
	Timer FireTimer;

	void FireProjectile(double rotation, SDL_Point direction);
};

