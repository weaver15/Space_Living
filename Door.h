#pragma once
#include <SDL.h>
#include <vector>
#include "AnimatedObject.h"

class Door : public AnimatedObject
{
public:
	Door();
	~Door();
	Door(int id, SDL_Rect textureInfo, SDL_Rect location, bool isVerticle, bool isBossDoor = false); 

	void CloseDoor(SDL_Rect* texture);
	void OpenDoor(SDL_Rect* texture);

	SDL_Rect InteractionArea;
	bool IsOpen = false;
	int OpenCost = 1;
	bool IsVertical; 
	bool IsBossDoor; 
	int Id; 
};

class DoorManager
{
public:
	DoorManager();
	~DoorManager();
	DoorManager(int numberOfDoors);

	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	void AddDoor(Door door);
	int CheckCollision(SDL_Rect* dimensions);
	bool CheckCollision(SDL_Rect* dimensions, int index);
	bool IsOpen(int index);
	int OpenDoor(SDL_Rect* dimensions, const int playerEnergy);
	bool DoorManager::OpenBossDoor(SDL_Rect* dimensions, bool hasKey);
	std::vector<Door> GetDoors();
	void SetTextureLocations(SDL_Rect vertClosedDoor, SDL_Rect vertOpenDoor, SDL_Rect horzClosedDoor, SDL_Rect horzOpenDoor);
	Door GetDoor(int index);
	void ResetDoors(); 
	std::vector<SDL_Rect*> GetDoorLocations();

private:
	int NumberOfDoors = 0;
	std::vector<Door> Doors;
	Door BossDoor; 
	SDL_Rect ClosedDoorVerticalTextureLocation;
	SDL_Rect OpenDoorVerticalTextureLocation;
	SDL_Rect ClosedDoorHorizontalTextureLocation;
	SDL_Rect OpenDoorHorizontalTextureLocation;
};