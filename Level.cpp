#pragma once
#include "Level.h"


Level::Level()
{
	CurrentLevel = 1;
}

Level::~Level()
{
	_Player.~Player();
}

#pragma region Load
bool Level::Load(SDL_Renderer* renderer)
{
	try
	{
		LoadLevel();
		LevelState = LevelState::Active;
		_Player.InitializeInventoryFont(renderer);
		_DialogBox.InitializeFont(renderer, 22, FontDialogBoxTag::DoorCost);
		LevelTimer.Start();
#ifdef _DEBUG
		printf("Level 1 Load Successful\n", SDL_GetError());
#endif
		return true;
	}
	catch (int n)
	{
#ifdef _DEBUG
		printf("Level load failed\n", SDL_GetError());
#endif
		ErrorLog.LogError(std::string("Level"), std::string("Load()"), std::string("Loading level failed."));
		return false;
	}
}
void Level::LoadLevel()
{

#ifdef _DEBUG
	printf("Starting Loading\n", SDL_GetError());
#endif

	//levelSize
	_GameInfoManager.LoadLevelInformation(&LevelSize);
	//Grid
	InitGrid(); 
	//Player
	_GameInfoManager.LoadPlayerInformation(&_Player);
	_GameInfoManager.LoadInventoryInformation(&_Player);
	//Boss
	 _GameInfoManager.LoadBossInformation(&_Boss);
	//Projectile
	_GameInfoManager.LoadProjectileInformation(&_Player, &_Boss);
	//Dialog Box
	_GameInfoManager.LoadDialogBoxInformation(&_DialogBox);
	//Items
	_GameInfoManager.LoadItemInformation(&_DroppedObjectManager);
	//Loot Table
	_GameInfoManager.LoadLootTableInformation(&_LootTable);
	//Enemy Manager 
	_GameInfoManager.LoadEnemyInformation(&_EnemyManager, &_SpawnManager);
	//Solid Objects
	_GameInfoManager.LoadSolidObjects(&_SolidObjectManager);

	//InitTest();

#ifdef _DEBUG
	printf("End Loading\n", SDL_GetError());
#endif

}
bool Level::LoadMedia(SDL_Renderer* renderer, SDL_Window* gWindow)
{
	bool success = true;
	//Player texture
	texPlayer.InitTexture(renderer, gWindow, "Player");

	//Menu texture
	texMenu.InitTexture(renderer, gWindow, "Menu");

	//Enemy texture
	texEnemy.InitTexture(renderer, gWindow, "Enemy");

	//Tile texture
	textTiles.InitTexture(renderer, gWindow, "Tile");

	_HUD.InitializeFont(renderer, 0, 0);
	_HUD.SetStartOfLevelHUD(renderer, _Player.GetHitPoints(), _Player.GetEnergyCount());

#ifdef _DEBUG
	printf("Level: Texture Loaded\n", SDL_GetError());
#endif

	LoadingFinished = true;
	return success;
}
#pragma endregion

#pragma region Level Update
void Level::Update(SDL_Renderer* renderer, GameState* gameState)
{
	try
	{
		if (LevelState == LevelState::Active)
		{
			std::vector<SDL_Rect> playerSolids = GetSolids();
			UpdateEnemies(renderer);
			_Player.Update(&Camera, bCheckCollision, playerSolids);
			UpdateDisplays(renderer);
			UpdateGameState(renderer, gameState);
		}	
		SetCamera();
		Draw(renderer);
	}
	catch (int i)
	{
		ErrorLog.LogError("Level", "Update()", "Failed");
	}
}
void Level::UpdateEnemies(SDL_Renderer* renderer)
{
	CheckToStartSpawning();
	if (StartSpawning && bSpawnEnemies)
		SpawnEnemies();
	
	CollisionFlags collisionFlags = _EnemyManager.Update(_Player.GetLocation(), _Player.GetProjectileManager(), &_Grid, _Player.GetTrail());
	
	_Boss.Update(_Player.GetLocation(), _Player.GetProjectileManager(), GetAllSolids(), &Camera);
	
	CheckCollisionResults(renderer, collisionFlags);
}
void Level::UpdateDisplays(SDL_Renderer* renderer)
{
	//Show or hide door cost
	int index = _Grid._DoorManager.CheckCollision(_Player.GetLocation());
	if (index >= 0)
	{
		auto door = _Grid._DoorManager.GetDoor(index);
		_DialogBox.UpdateLocation(door.GetLocation(), FontDialogBoxTag::DoorCost);
		_DialogBox.SetIsAlive(true);
		_DialogBox.UpdateFont(renderer, "Open cost: " + std::to_string(door.OpenCost), FontDialogBoxTag::DoorCost);
	}
	else if (_DialogBox.IsAlive())
	{
		_DialogBox.SetIsAlive(false);
	}
}
void Level::UpdateGameState(SDL_Renderer* renderer, GameState* gameState)
{
	if (!_Player.IsAlive())
	{

		Reset(renderer);
		if(GoToGameOver)
			*(gameState) = GameState::GameOverScreen;
	}

	if (!_Boss.IsAlive())
	{
		Reset(renderer);
		*(gameState) = GameState::CreditsScreen;
	}
}
void Level::Draw(SDL_Renderer* renderer)
{
	_Grid.DrawTiles(renderer, &textTiles, &Camera);
	_EnemyManager.Draw(renderer, &texEnemy, &Camera);
	_SpawnManager.Draw(renderer, &textTiles, &Camera);
	_DroppedObjectManager.Draw(renderer, &textTiles, &Camera);
	_SolidObjectManager.Draw(renderer, &textTiles, &Camera);
	_Player.Draw(renderer, &texPlayer, &Camera);
	_Player.DrawInventory(renderer, &texMenu, &Camera);
	_Boss.Draw(renderer, &texEnemy, &Camera);
	_DialogBox.Draw(renderer, &texMenu, &Camera);
	_HUD.Draw(renderer);
}
void Level::SetCamera()
{
	auto playerDim = _Player.GetLocation();
	Camera.x = (playerDim->x + playerDim->w / 2) - SCREEN_WIDTH / 2;
	Camera.y = (playerDim->y + playerDim->h / 2) - SCREEN_HEIGHT / 2;
	if (Camera.x < 0)
		Camera.x = 0;
	if (Camera.y < 0)
		Camera.y = 0;
	if (Camera.x > LevelSize.x - Camera.w)
		Camera.x = LevelSize.x - Camera.w;
	if (Camera.y > LevelSize.y - Camera.h)
		Camera.y = LevelSize.y - Camera.h;
}
#pragma endregion

#pragma region Items
void Level::DropItem(SDL_Point* dropPoint)
{
	if (dropPoint != nullptr)
	{
		SDL_Rect itemLocation;
		itemLocation.x = dropPoint->x;
		itemLocation.y = dropPoint->y;
		_DroppedObjectManager.AddDroppedObject(itemLocation, DroppedObjectType::Item);
	}
}
void Level::PickUpItem()
{
	InventoryItem item = _LootTable.GetRandomItem();
	item.SetStats(CurrentLevel);
	_Player.PickUpItem(&item);
}
#pragma endregion

#pragma region Level
void Level::PauseLevel()
{
	if (LevelState == LevelState::Active)
		LevelState = LevelState::Paused;
	else if (LevelState == LevelState::Paused)
		LevelState = LevelState::Active;
}
void Level::Reset(SDL_Renderer* renderer)
{
	_Player.Reset();
	_HUD.SetStartOfLevelHUD(renderer, _Player.GetHitPoints(), _Player.GetEnergyCount());
	_EnemyManager.Reset();
	_SpawnManager.Reset();
	_Grid._DoorManager.ResetDoors();
	_DroppedObjectManager.Reset();

}
void Level::CheckCollisionResults(SDL_Renderer* renderer, CollisionFlags flags)
{
	//Check the collision flag results to determine if an item should drop, player gets hit
	//player picks up item and the players energy count

	if (flags.ProjectileHit)
	{
		DropItem(_EnemyManager.GetLastItemDropLocation());
		_Player.IncrementEnergyCount(_EnemyManager.GetDeadEnemiesEnergy());
		_HUD.UpdatePlayerEnergy(renderer, _Player.GetEnergyCount());
	}
	if (flags.HitPlayer)
	{
		_Player.IncrementHitPoints(-flags.HitPlayerDamage);
		_HUD.UpdatePlayerHitPoints(renderer, _Player.GetHitPoints());
	}

	auto type = _DroppedObjectManager.CheckCollision(_Player.GetLocation());
	if (type == DroppedObjectType::Key)
	{
		_Player.SetHasBossKey(true);
	}
	else if (type != DroppedObjectType::None)
	{
		PickUpItem();
	}
}
#pragma endregion 

#pragma region Player
std::vector<SDL_Rect> Level::GetSolids()
{
	std::vector<SDL_Rect> solids;
	//Doors
	for each (Door door in _Grid._DoorManager.GetDoors())
	{
		if (!door.IsOpen && SDL_HasIntersection(door.GetLocation(), &Camera))
		{
			solids.push_back(*door.GetLocation());
		}
	}

	//Tile Walls
	for each (auto wall in _Grid.GetWalledTilesOnCamera(Camera))
	{
		solids.push_back(wall);
	}

	//Solid Objects
	for each (auto obj in _SolidObjectManager.GetSolidObjectsOnScreen(Camera))
	{
		solids.push_back(obj);
	}
	return solids;
}
std::vector<SDL_Rect> Level::GetAllSolids()
{
	//Used for getting all solids for enemies since they will
	//navigate the level off camera

	std::vector<SDL_Rect> solids;
	//Doors
	for each (Door door in _Grid._DoorManager.GetDoors())
	{
		if (!door.IsOpen)
		{
			solids.push_back(*door.GetLocation());
		}
	}

	//Tile Walls
	for each (auto wall in _Grid.GetWalledTilesOnCamera(Camera))
	{
		solids.push_back(wall);
	}
	return solids;
}
void Level::TogglePlayerInventory(SDL_Renderer* renderer)
{
	if (_Player.IsAlive() && LevelState != LevelState::Paused)
	{
		_Player.ToggleInventory(renderer, &Camera);
		if (_Player.IsInventoryShowing())
			LevelState = LevelState::ActiveInventory;
		else
			LevelState = LevelState::Active;
	}
}
void Level::PlayerInput(SDL_Event* e, SDL_Renderer* renderer)
{
	SDL_Keycode keyPressed = e->key.keysym.sym;

	if (e->type == SDL_KEYDOWN && keyPressed == SDLK_i && e->key.repeat == 0)
	{
		TogglePlayerInventory(renderer);
	}

	if (e->type == SDL_KEYDOWN && keyPressed == SDLK_ESCAPE && e->key.repeat == 0)
	{
		PauseLevel();
	}

	if (e->type == SDL_KEYDOWN && keyPressed == SDLK_e && e->key.repeat == 0)
	{
		_Player.IncrementEnergyCount(_Grid._DoorManager.OpenDoor(_Player.GetLocation(), _Player.GetEnergyCount()));
		_HUD.UpdatePlayerEnergy(renderer, _Player.GetEnergyCount());

		if (_Grid._DoorManager.OpenBossDoor(_Player.GetLocation(), _Player.GetHasBossKey()))
		{
			ActivateBoss();
		}
	}

	//Debug & testing inputs
	if (e->type == SDL_KEYDOWN && keyPressed == SDLK_F1 && e->key.repeat == 0)
	{
		
		SDL_Rect* loc = _Player.GetLocation();
		SDL_Point tile = _Player.GetTileLocation(_GameInfoManager.GetTileSize());

		printf("Location %d,%d \n", loc->x, loc->y);
		printf("Tile Location %d,%d \n", tile.x, tile.y);
	}

	if (_Player.IsInventoryShowing())
	{
		_Player.PlayerInventoryInput(renderer, e);
	}
}
#pragma endregion 

#pragma region Enemy
void Level::CheckToStartSpawning()
{

	if (!StartSpawning && LevelTimer.GetTicks() > SPAWNDELAY)
		StartSpawning = true;
}

void Level::SpawnEnemies()
{
	try
	{
		std::vector<AvailableSpawnInfo> availableSpawnLocations = _SpawnManager.Update(&_Grid, _Player.GetLocation(), Camera);

		if (availableSpawnLocations.size() > 0)
		{
			for each (AvailableSpawnInfo loc in availableSpawnLocations)
			{
				EnemyDetails details;
				details.Location.h = 32;
				details.Location.w = 32;
				details.Location.x = loc.Location.x;
				details.Location.y = loc.Location.y;

				details.TextureInfo.h = 32;
				details.TextureInfo.w = 32;
				details.TextureInfo.x = 0;
				details.TextureInfo.y = 0;

				details.TotalRowColumn.x = 1;
				details.TotalRowColumn.y = 1;

				details.ParentSpawn = loc.SpawnId;

				_EnemyManager.AddEnemy(details);
			}
		}
	}
	catch (int i)
	{
		ErrorLog.LogError("Level", "SpawnEnemies()", "Failed");
	}

}

void Level::ActivateBoss()
{
	_Boss.Activate();
}

#pragma endregion 

#pragma region Grid

//Set the grids number of tiles x and y and tiles 
void Level::InitGrid()
{
	_GameInfoManager.LoadGrid(&_Grid);
	_GameInfoManager.LoadDoorInformation(&_Grid._DoorManager);
}

#pragma endregion 

#pragma region Test
void Level::InitTest()
{
	GridLocation gr_loc = { 7 ,11 };
	EnemyDetails details;
	details.Location.h = 32;
	details.Location.w = 32;

	SDL_Point loc = _Grid.GetTileLocationByGridLocation(gr_loc);
	details.Location.x = loc.x;
	details.Location.y = loc.y;

	details.TextureInfo.h = 32;
	details.TextureInfo.w = 32;
	details.TextureInfo.x = 0;
	details.TextureInfo.y = 0;

	details.TotalRowColumn.x = 1;
	details.TotalRowColumn.y = 1;

	//details.IsTest = true;

	_TestEnemy = Enemy(details);

	//_TestEnemy.AddToPath(SDL_Point{ 880,1680 });
	//_TestEnemy.AddToPath(SDL_Point{ 880,2320 });
}
#pragma endregion 