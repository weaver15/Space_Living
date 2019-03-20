#pragma once
#include "AnimatedObject.h"
#include "Timer.h"
#include "Projectile.h"


enum MovementSpeed
{
	Stop = 0,
	Slow = 1,
	Medium = 2,
	Fast = 3
};

class Character : public AnimatedObject
{
public:
	Character();
	Character(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol = { 1,1 });
	~Character();
	void IncrementHitPoints(int value);
	void InitialIzeProjectile(SDL_Rect* textureInfo, SDL_Point* rowColumn, int maxProjectiles);
	void MoveBack(int value);
	ProjectileManager* GetProjectileManager();
	const int GetHitPoints();
	const int GetAttackDamage();

protected:
	Direction MovingDirection;
	int MovementSpeed = 0;
	ProjectileManager Projectiles;
	int RateOfFire = 0;
	double Rotation = 0.0;
	int HitPoints;
	int StartingHitPoints;
	int AttackDamage;
	SDL_Rect CollisionRect;

};

