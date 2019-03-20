#include "Character.h"


Character::Character()
{
}
Character::Character(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol)
	:AnimatedObject(textureInfo, location, totalRowCol)
{

}
Character::~Character()
{
}

void Character::IncrementHitPoints(int value)
{
	HitPoints += value; 
	if (HitPoints <= 0)
		SetIsAlive(false);
}

void Character::InitialIzeProjectile(SDL_Rect* textureInfo, SDL_Point* rowColumn, int maxProjectiles)
{
	Projectiles = ProjectileManager(maxProjectiles);
	Projectiles.SetProjectileDetails(textureInfo, rowColumn);
}

void Character::MoveBack(int value)
{
	auto location = GetLocation();
	if (MovingDirection == Direction::Down)
		location->y = location->y - value; 
	else if (MovingDirection == Direction::Up)
		location->y = location->y + value;
	if (MovingDirection == Direction::Left)
		location->x = location->x + value;
	if (MovingDirection == Direction::Right)
		location->x = location->x - value;
	SetLocation(location);
}

ProjectileManager* Character::GetProjectileManager()
{
	return &Projectiles;
}

const int Character::GetHitPoints()
{
	return HitPoints;
}

const int Character::GetAttackDamage()
{
	return AttackDamage;
}