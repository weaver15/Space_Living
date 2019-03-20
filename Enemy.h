#pragma once
#include <stdlib.h>  
#include <vector>
#include <queue>
#include <tuple>
#include "Character.h"
#include "Projectile.h"
#include "Grid.h"
#include "SpawnPoint.h"
#include <thread>

enum EnemyState
{
	Search,
	Attack,
	Dead
};

enum EnemyAttackType
{
	Melee,
	Ranged
};

struct CollisionFlags
{
	bool HitPlayer = false;
	int HitPlayerDamage = 0;
	bool ProjectileHit = false;
};

struct EnemyDetails
{
	SDL_Rect TextureInfo;
	SDL_Rect Location;
	SDL_Point TotalRowColumn;
	int ParentSpawn; 
	int Id; 
};

class Enemy : public Character
{
public:
	Enemy();
	Enemy(EnemyDetails details);
	~Enemy();

	void Update(SDL_Rect* playerLocation, vector<SDL_Rect> solids);
	void Reset();
	bool DropItem();

	const int GetEnergy();
	const int GetId();

	void SetCurrentState(EnemyState state);
	void SetDestination(SDL_Point dest);
	void SetPath(queue<SDL_Point> path);

	Timer SearchTimer;

private:
	bool CheckCollision(vector<SDL_Rect> solids);
	void SearchUpdate(vector<SDL_Rect> solids);
	void SearchDestionationUpdate();
	void SearchMovementUpdate(vector<SDL_Rect> solids);

	EnemyState CurrentState;
	EnemyAttackType AttackType;
	SDL_Point Destination;

	int Id;
	int Energy;
	int DropItemChance = 25;
	int ParentSpawn;
	bool IsMovingToDestination = false;
	int SearchWaitTime = 500;
	queue<SDL_Point> Path;
};


class EnemyManager
{
public:
	EnemyManager();
	EnemyManager(size_t numberOfEnemies);
	~EnemyManager();
	void InitializeEnemies(std::vector<EnemyDetails> details);
	CollisionFlags Update(SDL_Rect* playerLocation, ProjectileManager* projectiles,
		Grid* grid, std::vector<SDL_Point>* path);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera);
	void AddEnemy(EnemyDetails details);
	void RemoveEnemy(int index);
	SDL_Point* GetLastItemDropLocation();
	int GetDeadEnemiesEnergy();
	queue<SDL_Point> AdjustPath(SDL_Rect* location, std::vector<SDL_Point>* path, Grid* grid);
	void Reset();

private:

	ErrorManager ErrorLog;
	Calculate Calculator;
	vector<Enemy> Enemies;

	int PushbackDistance = 15;
	int SearchWaitTime = 250;
	float RemoveDistance = 750;

	int NumberOfEnemies;
	int AliveEnemies;
	int LastDeadEnemiesEnergy;
	SDL_Point LastItemDrop;

	int FirstDeadEnemy();
	void UpdatePlayerHit(int index, CollisionFlags* collisionFlag);
	void UpdateSearch(int index, std::vector<SDL_Point>* path, Grid* grid);
	void UpdateAttack(int index, SDL_Rect* playerLocation);
	vector<SDL_Rect> GetEnemyLocations(int exclude);
};