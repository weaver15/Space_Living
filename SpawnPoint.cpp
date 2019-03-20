#pragma once
#include "SpawnPoint.h"

SpawnPoint::SpawnPoint()
{
	SetTag("SpawnPoint");
}

SpawnPoint::~SpawnPoint()
{
}

SpawnPoint::SpawnPoint(SDL_Rect textureInfo, SDL_Rect location, int spawnRate, int doorId, SDL_Point totalRowCol)
	: AnimatedObject(textureInfo, location, totalRowCol)
{
	SetTag("SpawnPoint");
	//SetIsAlive(true);
	SpawnRate = spawnRate;
	BehindDoorId = doorId;
}

void SpawnPoint::Update(Grid* grid, SDL_Rect* playerLocation, SDL_Rect camera)
{
	//Wait to spawn another wave
	if (EnemiesSpawned >= SpawnWave)
	{
		EnemiesSpawned = 0;
		WaveTimer.Start();
	}

	//Update Spawn Point, if it has waited long enough set it ready to spawn
	if (WaveTimer.IsStarted())
	{
		if (WaveTimer.GetTicks() > WaveWaitTime)
		{
			WaveTimer.Reset();
			ReadyToSpawn = true; 
		}
		else
		{
			ReadyToSpawn = false; 
		}
	}
	else if (SpawnWait.IsStarted() && SpawnWait.GetTicks() > SpawnRate)
	{
		SpawnWait.Reset();
		ReadyToSpawn = true;
	}
	//Update the path for enemies spawned from this point.

	//UpdateSearch(grid, playerLocation, camera);
}

void SpawnPoint::UpdateSearch(Grid* grid, SDL_Rect* playerLocation, SDL_Rect camera)
{
	//TODO stop if this isn't spawning and has no children enemies spawned
	if (!SearchTimer.IsStarted())
	{
		Path = grid->GetPath(GetLocation(), playerLocation);
		SearchTimer.Start();
	}
	if (SearchTimer.IsStarted() && SearchTimer.GetTicks() > SearchWaitTime)
		SearchTimer.Reset();
}

void SpawnPoint::StartSpawning()
{
	ReadyToSpawn = true;
}

const int SpawnPoint::BehindDoor()
{
	return BehindDoorId;
}

const bool SpawnPoint::IsReadyToSpawn()
{
	return ReadyToSpawn;
}

const int SpawnPoint::GetId()
{
	return Id;
}

void SpawnPoint::SetId(int id)
{
	Id = id;
}

SDL_Rect SpawnPoint::SpawnEnemy()
{
	//Spawn point is no longer ready to spawn, start its wait time and return its location for new
	//enemy to be spawned at

	ReadyToSpawn = false;
	SpawnWait.Start();
	EnemiesSpawned++;
	return Location;
}

SpawnPointManager::SpawnPointManager()
{
	SpawnPoints.resize(0);
}
SpawnPointManager::~SpawnPointManager()
{
}
SpawnPointManager::SpawnPointManager(size_t maxNumberOfSpawnPoints)
{
	SpawnPoints.resize(0);
	MaxNumberOfSpawnPoints = maxNumberOfSpawnPoints;
}

std::vector<AvailableSpawnInfo> SpawnPointManager::Update(Grid* grid, SDL_Rect* playerLocation, SDL_Rect camera)
{
	//Update spawn points and return a list of points ready to spawn
	//if close enough to the player.
	std::vector<AvailableSpawnInfo> availableSpawnLocations;
	for (auto& spawn : SpawnPoints)
	{
		//is the spawn point is behind a closed door don't spawn there
		bool openDoor = false;
		if (!spawn.IsAlive() && spawn.BehindDoor() > 0)
		{
			openDoor = grid->_DoorManager.IsOpen(spawn.BehindDoor());
		}
		else
		{
			openDoor = true;
		}

		//if the spawn is not behind a closed door update it
		if (openDoor)
		{
			spawn.SetIsAlive(Calculator.WithinDistance(spawn.GetLocation(), playerLocation, RemoveDistance));
			if (spawn.IsAlive())
			{
				spawn.Update(grid, playerLocation, camera);
				if (spawn.IsReadyToSpawn())
				{
					auto location = spawn.SpawnEnemy();
					AvailableSpawnInfo info;
					info.Location.x = location.x;
					info.Location.y = location.y;
					info.SpawnId = spawn.GetId();
					availableSpawnLocations.push_back(info);
				}
			}
		}

	}
	return availableSpawnLocations;
}

void SpawnPointManager::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	for (auto spawn : SpawnPoints)
	{
		if (spawn.IsAlive())
		{
			spawn.Draw(renderer, texture, camera);
		}
	}
}

void SpawnPointManager::AddSpawnPoints(SDL_Rect textureInfo, SDL_Rect location, int spawnRate, int doorId, SDL_Point totalRowCol)
{
	SpawnPoint spawn = SpawnPoint(textureInfo, location, spawnRate, doorId, totalRowCol);
	spawn.StartSpawning();
	spawn.SetId(NextSpawnId);
	NextSpawnId++;
	SpawnPoints.push_back(spawn);
}

void SpawnPointManager::Reset()
{
	for (auto& spawn : SpawnPoints)
	{
		spawn.StartSpawning();
		spawn.SpawnWait.Reset();
	}
}