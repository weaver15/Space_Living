#pragma once
#include "Enemy.h"

#pragma region Enemy
Enemy::Enemy()
{
	CurrentState = EnemyState::Dead;
	SetTag("Enemy");
}

Enemy::Enemy(EnemyDetails details)
	:Character(details.TextureInfo, details.Location, details.TotalRowColumn)
{
	CurrentState = EnemyState::Search;
	AttackType = EnemyAttackType::Melee;
	MoveFrame(Direction::Left);
	MovementSpeed = MovementSpeed::Fast;
	Energy = 1;
	MovingDirection = Direction::Left;
	ParentSpawn = details.ParentSpawn;
	AttackDamage = 2;
	HitPoints = 3;
	SetTag("Enemy");
	SetIsAlive(true);
	Id = details.Id; 
}

Enemy::~Enemy()
{
	CurrentState = EnemyState::Dead;
	MovementSpeed = MovementSpeed::Stop;
	SetIsAlive(false);
}

void Enemy::Update(SDL_Rect* playerLocation, vector<SDL_Rect> solids)
{
	switch (CurrentState)
	{
		case Search:
			SearchUpdate(solids);
			break;
		case Attack:
			SearchMovementUpdate(solids);
			break;
		case Dead:
			break;
		default:
			break;
	}
}

const int Enemy::GetEnergy()
{
	return Energy;
}
const int Enemy::GetId()
{
	return Id;
}

void Enemy::SetCurrentState(EnemyState state)
{
	CurrentState = state; 
}

void Enemy::SetDestination(SDL_Point dest)
{
	Destination = dest;
}

void Enemy::SearchUpdate(vector<SDL_Rect> solids)
{
	if (!IsMovingToDestination)
		SearchDestionationUpdate();
	else
		SearchMovementUpdate(solids);
}
void Enemy::SearchDestionationUpdate()
{
	if (Path.size() > 0)
	{
		Destination = Path.front();
		Path.pop();
		IsMovingToDestination = true;
	}
}
void Enemy::SearchMovementUpdate(vector<SDL_Rect> solids)
{
	bool reachedX = false;
	bool reachedY = false;
	SDL_Rect start = Location; 

	if ((Location.x - MovementSpeed) > Destination.x)
	{
		Location.x -= MovementSpeed;
		MovingDirection = Direction::Left;
	}
	else if ((Location.x + MovementSpeed) < Destination.x)
	{
		Location.x += MovementSpeed;
		MovingDirection = Direction::Right;
	}
	else
		reachedX = true;

	if ((Location.y - MovementSpeed) > Destination.y)
	{
		Location.y -= MovementSpeed;
		MovingDirection = Direction::Up;
	}
	else if ((Location.y + MovementSpeed) < Destination.y)
	{
		Location.y += MovementSpeed;
		MovingDirection = Direction::Down;
	}
	else
		reachedY = true;

	//Check Collision with other enemies so they don't stack 
	if (CheckCollision(solids))
	{
		Location = start;
		reachedX = false;
		reachedY = false;
	}

	if (reachedY && reachedX)
		IsMovingToDestination = false;

	MoveFrame(MovingDirection);
}

bool Enemy::CheckCollision(vector<SDL_Rect> solids)
{
	if (solids.size() > 0)
	{
		for (auto solid : solids)
		{
			if (SDL_HasIntersection(&Location, &solid))
			{
				return true;
			}
		}
	}
	return false;
}

void Enemy::Reset()
{
	AnimatedObject::Reset();
	CurrentState = EnemyState::Search;
	MovingDirection = Direction::Left;
	MoveFrame(Direction::Left);
}

bool Enemy::DropItem()
{
	int roll = rand() % 100;
	if (roll < DropItemChance)
		return true;
	else
		return false; 
}

void Enemy::SetPath(queue<SDL_Point> path)
{
	Path = queue<SDL_Point>{ path };
}
#pragma endregion

#pragma region Enemy Manager
EnemyManager::EnemyManager()
{
	Enemies.resize(1);
	NumberOfEnemies = 1;
}

EnemyManager::EnemyManager(size_t numberOfEnemies)
{
	Enemies.resize(numberOfEnemies);
	NumberOfEnemies = numberOfEnemies;
	AliveEnemies = 0;
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::InitializeEnemies(std::vector<EnemyDetails> details)
{
	for (int index = 0; index < Enemies.size(); index++)
	{
		Enemies[index] = Enemy(details[index]);
	}
}

CollisionFlags EnemyManager::Update(SDL_Rect* playerLocation,
	ProjectileManager* projectiles, Grid* grid, std::vector<SDL_Point>* path)
{
	CollisionFlags collisionFlags;

	if (AliveEnemies > 0)
	{
		//Get alive enemies
		for (int index = 0; index < NumberOfEnemies; index++)
		{
			if (projectiles->CheckCollision(Enemies[index].GetLocation()))
			{
				Enemies[index].IncrementHitPoints(-(projectiles->GetDamage()));
				if (Enemies[index].GetHitPoints() <= 0)
				{
					LastItemDrop = { 0,0 };
					if (Enemies[index].DropItem())
					{
						auto enemyLocation = Enemies[index].GetLocation();
						LastItemDrop = { enemyLocation->x, enemyLocation->y };
					}
					LastDeadEnemiesEnergy = Enemies[index].GetEnergy();
					RemoveEnemy(index);
					collisionFlags.ProjectileHit = true;
				}
				else
				{
					Enemies[index].MoveBack(PushbackDistance);
				}
			}
			else if (SDL_HasIntersection(Enemies[index].GetLocation(), playerLocation))
			{
				UpdatePlayerHit(index, &collisionFlags);
			}
			else if (Enemies[index].IsAlive())
			{
				//Remove enemies that are far away from player
				auto withinDistance = Calculator.WithinDistance(Enemies[index].GetLocation(), playerLocation, RemoveDistance);
				if (!withinDistance)
				{
					RemoveEnemy(index);
					break;
				}

				if (grid->IsStraightPathObstructed(Enemies[index].GetLocation(), playerLocation))
				{
					UpdateSearch(index, path, grid);
				}
				else
				{
					UpdateAttack(index, playerLocation);
				}

				auto otherEnemies = GetEnemyLocations(Enemies[index].GetId());
				Enemies[index].Update(playerLocation, otherEnemies);
			}
		}
	}
	return collisionFlags;
}

vector<SDL_Rect> EnemyManager::GetEnemyLocations(int exclude)
{
	vector<SDL_Rect> locs = vector<SDL_Rect>();
	for (auto enemy : Enemies)
	{
		if (enemy.IsAlive() && enemy.GetId() != exclude)
		{
			locs.push_back(*enemy.GetLocation());
		}
	}
	return locs;
}

queue<SDL_Point> EnemyManager::AdjustPath(SDL_Rect* location, std::vector<SDL_Point>* path, Grid* grid)
{
	SDL_Point enemyLocation = SDL_Point{ location->x, location->y };
	//Find the index of the enemies location in the path to see where to start
	//index will be -1 if it doesn't exist
	int index = -1;
	for (int i = 0; i < path->size(); i++)
	{
		if (path->at(i).x == enemyLocation.x && path->at(i).y == enemyLocation.y)
		{
			index = i;
			break;
		}
	}

	std::queue<SDL_Point> enemyPath;
	//If location doesn't exist find the closest
	if (index < 0)
	{
		int currentClosestDistance = Calculator.Distance(&enemyLocation, &path->at(0));
		int currentClosestIndex = 0;
		for (int i = 1; i < path->size(); i++)
		{
			int newDistance = Calculator.Distance(&enemyLocation, &path->at(i));
			if (newDistance < currentClosestDistance)
			{
				currentClosestDistance = newDistance;
				currentClosestIndex = i;
			}
		}
		index = currentClosestIndex;
	}

	//If path to start point is obstructed A* to it
	if (grid->IsStraightPathObstructed(&enemyLocation, &path->at(index)))
	{

	}

	std::vector<SDL_Point> part(path->begin() + index, path->end());
	for (auto value : part)
	{
		enemyPath.push(value);
	}
	return enemyPath;
}

void EnemyManager::UpdateSearch(int index, std::vector<SDL_Point>* path, Grid* grid)
{
	if (!Enemies[index].SearchTimer.IsStarted())
	{
		Enemies[index].SetCurrentState(EnemyState::Search);
		Enemies[index].SetPath(AdjustPath(Enemies[index].GetLocation(), path, grid));
		Enemies[index].SearchTimer.Start();
	}
	if (Enemies[index].SearchTimer.IsStarted() && Enemies[index].SearchTimer.GetTicks() > SearchWaitTime)
		Enemies[index].SearchTimer.Stop();
}

void EnemyManager::UpdateAttack(int index, SDL_Rect* playerLocation)
{
	Enemies[index].SetCurrentState(EnemyState::Attack);
	Enemies[index].SetDestination(SDL_Point{ playerLocation->x, playerLocation->y });
}

void EnemyManager::UpdatePlayerHit(int index, CollisionFlags* collisionFlag)
{
	CollisionFlags flags;
	auto enemyLocation = Enemies[index].GetLocation();
	Enemies[index].MoveBack(PushbackDistance);
	collisionFlag->HitPlayer = true;
	collisionFlag->HitPlayerDamage = Enemies[index].GetAttackDamage();
}

void EnemyManager::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	for (auto enemy : Enemies)
	{
		enemy.Draw(renderer, texture, camera);
	}
}

void EnemyManager::AddEnemy(EnemyDetails detail)
{
	if (AliveEnemies < NumberOfEnemies)
	{
		int index = FirstDeadEnemy();
		detail.Id = index;
		Enemies[index] = Enemy(detail);
		AliveEnemies++;
	}
}

void EnemyManager::RemoveEnemy(int index)
{
	if (AliveEnemies > 0)
	{
		Enemies[index].~Enemy();
		AliveEnemies--;
	}

}

int EnemyManager::FirstDeadEnemy()
{
	for (size_t i = 0; i < Enemies.size(); i++)
	{
		if (!Enemies[i].IsAlive())
			return i;
	}
}

SDL_Point* EnemyManager::GetLastItemDropLocation()
{
	if (LastItemDrop.x != 0 && LastItemDrop.y != 0)
		return &LastItemDrop;
	else
		return nullptr;
}

int EnemyManager::GetDeadEnemiesEnergy()
{
	return LastDeadEnemiesEnergy;
}

void EnemyManager::Reset()
{
	Enemies = std::vector<Enemy>();
	Enemies.resize(NumberOfEnemies);
	AliveEnemies = 0;
}

#pragma endregion 