#pragma once
#include "pugixml\pugixml.hpp"
#include <SDL.h>
#include <sstream>
#include <vector>

#include "Tile.h"
#include "Boss.h"
#include "Enemy.h"
#include "SolidObject.h"
#include "Door.h"
#include "SpawnPoint.h"
#include "DroppedObject.h"
#include "LootTable.h"
#include "Player.h"


//Manager for Loading and Saving information to outside xml
class GameInfoManager
{
public:
	GameInfoManager();
	~GameInfoManager();

	int LoadTileSize();
	const int GetTileSize();

	//Loading
	void LoadLevelInformation(SDL_Point* level);
	void LoadPlayerInformation(Player* player);
	std::vector<SDL_Rect> LoadTileTextureLocations();
	std::vector<int> LoadTileLayout();
	void LoadBossInformation(Boss* boss);
	void GameInfoManager::LoadDialogBoxInformation(FontDialogBox* font);
	void LoadSolidObjects(SolidObjectManager* manager);
	void LoadDoorInformation(DoorManager* doorManager);
	void LoadProjectileInformation(Player* player, Boss* boss);
	void LoadEnemyInformation(EnemyManager* enemyManager, SpawnPointManager* spawnManager);
	void LoadGrid(Grid* grid);
	//Saving

private:
	pugi::xml_document Doc;
	pugi::xml_node Level;
	int TileSize;
	int ScreenTileSize;
};

