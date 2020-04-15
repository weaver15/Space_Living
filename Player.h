#pragma once
#include <SDL.h>
#include <stdio.h>
#include <string>
#include "Door.h"
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
	//Reset
	void Reset();
	vector<SDL_Point>* GetTrail();
	void ControllerInput();

private:
	int WalkSpeed = 6;
	int RunSpeed = 8; 
	static int const MAXTRAIL = 100;
	vector<SDL_Point> Trail;
	void AddToTrail();
	Timer BulletTimer; 
};

