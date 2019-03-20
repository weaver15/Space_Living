#pragma once
#include <SDL.h>
#include <vector>
#include "AnimatedObject.h"
#include "Timer.h"
#include "Grid.h"

struct AvailableSpawnInfo
{
	SDL_Point Location;
	int SpawnId; 
};

class SpawnPoint final : public AnimatedObject
{
public:
	SpawnPoint();
	SpawnPoint(SDL_Rect textureInfo, SDL_Rect location, int spawnRate, int doorId, SDL_Point totalRowCol);
	~SpawnPoint();


	void Update(Grid* grid, SDL_Rect* playerLocation, SDL_Rect camera);
	void UpdateSearch(Grid* grid, SDL_Rect* playerLocation, SDL_Rect camera);
	void StartSpawning();
	const bool IsReadyToSpawn();
	SDL_Rect SpawnEnemy();
	const int BehindDoor();
	const int GetId();
	void SetId(int id);

	Timer SpawnWait;
private:
	int Id;
	int WaveWaitTime = 30000;
	int SpawnRate = 0; 
	int EnemiesSpawned = 0; 
	int SpawnWave = 3;
	int SearchWaitTime = 1000;
	int BehindDoorId = 0;
	bool ReadyToSpawn = false;
	std::vector<SDL_Point> Path;
	Timer WaveTimer;
	Timer SearchTimer;
};

class SpawnPointManager
{
public:
	SpawnPointManager();
	SpawnPointManager(size_t maxNumberOfSpawnPoints);
	~SpawnPointManager();

	std::vector<AvailableSpawnInfo> Update(Grid* grid, SDL_Rect* playerLocation, SDL_Rect camera);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	void AddSpawnPoints(SDL_Rect textureInfo, SDL_Rect location, int spawnRate, int doorId, SDL_Point totalRowCol = {1,1});
	void Reset();
private:
	std::vector<SpawnPoint> SpawnPoints;
	int MaxNumberOfSpawnPoints = 0;
	int NextSpawnId = 0;
	float RemoveDistance = 750;
	Calculate Calculator;
};

