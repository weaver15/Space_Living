#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "Door.h"
#include "Inventory.h"
#include "Globals.cpp"
#include "Character.h"

enum PlayerMovementState
{
	Walking,
	Running
};

class Player : public Character
{
public:
	Player();	
	~Player();
	Player(std::pair<SDL_Rect, SDL_Rect> details);
	void Update(SDL_Rect * camera, bool checkCollision, std::vector<SDL_Rect> solids);
	void Player::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera);
	void DrawInventory(SDL_Renderer* renderer, Texture* texture,  SDL_Rect * camera);

	//Add item on ground to inventory
	void PickUpItem(InventoryItem* item); 

	//Inventory
	void ToggleInventory(SDL_Renderer* renderer, SDL_Rect * camera);
	void PlayerInventoryInput(SDL_Renderer* renderer, SDL_Event* e);
	bool IsInventoryShowing() { return PlayerInventory.IsInventoryShowing(); }


	//Initializers 
	void InitializeInventory(InventoryInformation inventoryInfo, EquipmentSlotInformation equipInfo);
	void InitializeInventoryFont(SDL_Renderer* renderer);

	//Reset
	void Reset();

	vector<SDL_Point>* GetTrail();

	void ControllerInput();

private:
	int WalkSpeed = 6;
	int RunSpeed = 8; 
	static int const MAXTRAIL = 100;
	vector<SDL_Point> Trail;

	Timer BulletTimer; 
	Inventory PlayerInventory;
	Stats PlayerStats; 
	
	void AddToTrail();
};

