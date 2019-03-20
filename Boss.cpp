#include "Boss.h"



Boss::Boss()
{
	SetTag("Boss");
}

Boss::Boss(std::pair<SDL_Rect, SDL_Rect> details)
	: Character(details.first, details.second)
{

	//TODO rate of fire should be read in
	RateOfFire = 500;
	IsActive = false; 
	SetTag("Boss");
	SetIsAlive(true);
}

Boss::~Boss()
{

}

void Boss::Update(SDL_Rect* playerLocation, ProjectileManager* projectiles, std::vector<SDL_Rect> solids, SDL_Rect * camera)
{
	if (IsActive)
	{
		if (projectiles->CheckCollision(GetLocation()))
		{
			Projectiles.Clear();
			IsActive = false; 
			SetIsAlive(false);
		}
		else
		{
			SDL_Point delta = { GetLocation()->x - playerLocation->x, GetLocation()->y - playerLocation->y };
			Rotation = ((atan2(delta.y, delta.x) * 180.0000) / 3.1416) + 90.0000;
			
			if (!FireTimer.IsStarted())
			{
				FireTimer.Start();
				SDL_Point diretion = delta;
				
				FireProjectile(Rotation - 90.0000, diretion);
			}
			else
			{
				if (FireTimer.GetTicks() > RateOfFire)
					FireTimer.Stop();
			}
		}
		Projectiles.Update(camera, solids);
	}
}

void Boss::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera)
{
	Projectiles.Draw(renderer, texture, camera);
	AnimatedObject::Draw(renderer, texture, camera, Rotation);
}

void Boss::FireProjectile(double rotation, SDL_Point diretion)
{
	Projectiles.FireProjectile(&Location, diretion, rotation);
}

void Boss::Activate()
{
	IsActive = true; 
}