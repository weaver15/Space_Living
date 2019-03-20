#pragma once
#include "Projectile.h"

Projectile::Projectile()
{
	SetTag("Projectile");
}


Projectile::~Projectile()
{
}

Projectile::Projectile(ProjectileDetails details)
	:AnimatedObject(details.m_textureInfo, details.m_location, details.m_totalRowColumn)
{
	MovementDirection = details.m_movementDirection;
	MovementPoint = details.movementPoint; 
	Velocity = details.Velocity; 
	MoveFrame(MovementDirection);
	MovementSpeed = 15;
	SetRotation(details.Rotation);
	SetIsAlive(true);
}

void Projectile::Update(int* _bulletCount, SDL_Rect * camera, std::vector<SDL_Rect> solids)
{
	if (IsAlive())
	{
		if (!SDL_HasIntersection(&Location, camera))
		{
			SetIsAlive(false);
			(*_bulletCount) = (*_bulletCount) - 1;
		}
		else if (MovementDirection == Direction::Down)
			Location.y = Location.y + MovementSpeed;
		else if (MovementDirection == Direction::Up)
			Location.y = Location.y - MovementSpeed;
		else if (MovementDirection == Direction::Left)
			Location.x = Location.x - MovementSpeed;
		else if (MovementDirection == Direction::Right)
			Location.x = Location.x + MovementSpeed;
		else if (MovementDirection == Direction::UpLeft)
		{
			Location.x = Location.x - MovementSpeed;
			Location.y = Location.y - MovementSpeed;
		}
		else if (MovementDirection == Direction::UpRight)
		{
			Location.x = Location.x + MovementSpeed;
			Location.y = Location.y - MovementSpeed;
		}
		else if (MovementDirection == Direction::DownLeft)
		{
			Location.x = Location.x - MovementSpeed;
			Location.y = Location.y + MovementSpeed;
		}
		else if (MovementDirection == Direction::DownRight)
		{
			Location.x = Location.x + MovementSpeed;
			Location.y = Location.y + MovementSpeed;
		}
		else if (MovementDirection == Direction::NoDirection)
		{
			Location.x = Location.x - Velocity.first;
			Location.y = Location.y - Velocity.second;
		}
		if (CheckCollision(solids))
		{
			SetIsAlive(false);
			(*_bulletCount) = (*_bulletCount) - 1;
		}
	}
}

Direction Projectile::GetMovementDirection()
{
	return MovementDirection;
}

double Projectile::GetRotation() const
{
	return Rotation; 
}
void Projectile::SetRotation(double rotation)
{
	Rotation = rotation;
}



ProjectileManager::ProjectileManager()
{
	Projectiles.resize(1);
	BulletCount = 0;
}

ProjectileManager::ProjectileManager(int maxNumberOfProjectiles)
{
	Projectiles.resize(maxNumberOfProjectiles);
	BulletCount = 0;
	MaxNumberOfProjectiles = maxNumberOfProjectiles;
}


ProjectileManager::~ProjectileManager()
{
}

void ProjectileManager::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera)
{
	if (BulletCount > 0)
	{
		for (size_t index = 0; index < Projectiles.size(); index++)
		{
			if (Projectiles[index].IsAlive())
			{
				Direction direction = Projectiles[index].GetMovementDirection();
				Projectiles[index].Draw(renderer, texture, camera, Projectiles[index].GetRotation());
			}
		}
	}
}

void ProjectileManager::Update(SDL_Rect * camera, std::vector<SDL_Rect> solids)
{
	if (BulletCount > 0)
	{
		for (size_t i = 0; i < Projectiles.size(); i++)
		{
			Projectiles[i].Update(&BulletCount, camera, solids);
		}
	}
}

int ProjectileManager::FirstDeadProjectile()
{
	for (size_t i = 0; i < sizeof(Projectiles); i++)
	{
		if (!Projectiles[i].IsAlive())
			return i;
	}
}

void ProjectileManager::AddProjectile(ProjectileDetails details)
{
	if (BulletCount < MaxNumberOfProjectiles)
	{
		int index = FirstDeadProjectile();
		Projectiles[index] = Projectile(details);
		BulletCount +=1;
	}
}

bool ProjectileManager::CheckCollision(SDL_Rect* locationRec)
{
	if (BulletCount > 0)
	{
		for(auto &projectile : Projectiles)
		{
			if (projectile.CheckCollision(locationRec))
			{
				projectile.SetIsAlive(false);
				BulletCount -= 1;
				return true;
			}
		}
	}
	return false;
}

void ProjectileManager::FireProjectile(SDL_Rect* startLocation, Direction movementDirection)
{

	Details.m_location.x = startLocation->x;
	Details.m_location.y = startLocation->y;

	Details.m_movementDirection = movementDirection;

	Details.Rotation = 0.0;
	if (movementDirection == Direction::Up || movementDirection == Direction::Down)
		Details.Rotation = 90.0;
	else  if (movementDirection == Direction::UpLeft || movementDirection == Direction::DownRight)
		Details.Rotation = 45.0;
	else  if (movementDirection == Direction::UpRight || movementDirection == Direction::DownLeft)
		Details.Rotation = -45.0;


	AddProjectile(Details);
}

//Boss fire projectile
void ProjectileManager::FireProjectile(SDL_Rect* startLocation, SDL_Point movementDirection, double rotation)
{
	int bossBulletMovementSpeed = 15;
	Details.m_location.x = startLocation->x;
	Details.m_location.y = startLocation->y;
	Details.Rotation = rotation;
	Details.m_movementDirection = Direction::NoDirection;
	Details.movementPoint = movementDirection;
	Details.Velocity = Calculator.CalculateVelocity(movementDirection.x, movementDirection.y, bossBulletMovementSpeed);
	AddProjectile(Details);
}

void ProjectileManager::SetProjectileDetails(SDL_Rect* textureInfo, SDL_Point* rowCol)
{
	Details.m_textureInfo.x = textureInfo->x;
	Details.m_textureInfo.y = textureInfo->y;
	Details.m_textureInfo.w = textureInfo->w;
	Details.m_textureInfo.h = textureInfo->h;
	Details.m_totalRowColumn = { rowCol->x,rowCol->y };
	Details.m_location.w = textureInfo->w;
	Details.m_location.h = textureInfo->h;
}

void ProjectileManager::SetDamage(int damage)
{
	Damage = damage;
}

void ProjectileManager::Clear()
{
	Projectiles.clear();
}