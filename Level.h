#pragma once
#include <sstream>
#include "Globals.cpp"
#include "HUD.h"
#include "GameInfoManager.h"

enum LevelState
{
	Active,
	Paused,
	ActiveInventory
};

class Level
{
public:
	Level();
	~Level();

	void Update(SDL_Renderer* renderer, GameState* gameState);
	void Draw(SDL_Renderer* renderer);
	bool Load(SDL_Renderer* renderer);
	bool LoadMedia(SDL_Renderer* renderer, SDL_Window* gWindow);
	void PlayerInput(SDL_Event* e, SDL_Renderer* renderer);
	bool IsLoaded() const { return LoadingFinished; }

private:
	int CurrentLevel;
	bool LoadingFinished = false;
	bool StartSpawning = false;
	//False will just reset the game after death, True will go to game over screen
	bool GoToGameOver = false;

	//Delay spawning at the beginning of the level, 10000 = 10 sec
	int const SPAWNDELAY = 10000;
	LevelState LevelState;
	SDL_Point LevelSize;
	SDL_Rect Camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//Textures
	Texture texMenu;
	Texture texPlayer;
	Texture texEnemy;
	Texture textTiles;

	//Managers
	Grid _Grid;
	Player _Player;
	EnemyManager _EnemyManager;
	Boss _Boss; 
	SpawnPointManager _SpawnManager;
	SolidObjectManager _SolidObjectManager; 
	LootTable _LootTable; 
	HUD _HUD;
	Timer LevelTimer;
	FontDialogBox _DialogBox; 
	GameInfoManager _GameInfoManager;
	ErrorManager ErrorLog;

	//Helpers
	void LoadLevel();
	void Reset(SDL_Renderer* renderer);
	void PauseLevel();
	void SetCamera();
	void CheckCollisionResults(SDL_Renderer* renderer, CollisionFlags flags);
	void CheckToStartSpawning();
	void SpawnEnemies();
	void InitGrid();
	void ActivateBoss();
	void InitTest();
	void UpdateEnemies(SDL_Renderer* renderer);
	void UpdateGameState(SDL_Renderer* renderer, GameState* gameState);
	std::vector<SDL_Rect> GetSolids();
	std::vector<SDL_Rect> GetAllSolids();

	//Test
	bool bCheckCollision = true;
	bool bSpawnEnemies = true;
	Enemy _TestEnemy;
};

