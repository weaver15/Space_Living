#pragma once
#include "Player.h"

Player::Player()
{
	SetTag("Player");
}
Player::Player(std::pair<SDL_Rect, SDL_Rect> details)
	:Character(details.first, details.second)
{
	MovingDirection = Direction::Down;
	RateOfFire = 150;
	StartingHitPoints = 45;
	HitPoints = 45; 
	SetIsAlive(true);
	Trail.resize(MAXTRAIL);
	SetTag("Player");
}

Player::~Player()
{
	Projectiles.~ProjectileManager();
	BulletTimer.~Timer();
	RateOfFire = 0;
	MovementSpeed = 0;
}

void Player::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera)
{
	Projectiles.Draw(renderer, texture, camera);
	AnimatedObject::Draw(renderer, texture, camera, Rotation);
}


void Player::Update(SDL_Rect * camera, bool checkCollision, std::vector<SDL_Rect> solids)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	//Left Shift to run
	if (currentKeyStates[SDL_SCANCODE_LSHIFT])
		MovementSpeed = RunSpeed;
	else
		MovementSpeed = WalkSpeed;

	//Controls
	if (currentKeyStates[SDL_SCANCODE_W])
	{
		if (currentKeyStates[SDL_SCANCODE_A])
			Rotation = 45.0;
		else if (currentKeyStates[SDL_SCANCODE_D])
			Rotation = -45.0;
		else
			Rotation = 0.0;
		MovingDirection = Direction::Up;
		Location.y -= MovementSpeed;
		if (checkCollision && CheckCollision(solids))
			Location.y += MovementSpeed;

	}
	if (currentKeyStates[SDL_SCANCODE_S])
	{
		if (currentKeyStates[SDL_SCANCODE_A])
			Rotation = -45.0;
		else if (currentKeyStates[SDL_SCANCODE_D])
			Rotation = 45.0;
		else
			Rotation = 0.0;
		MovingDirection = Direction::Down;
		Location.y += MovementSpeed;
		if (checkCollision && CheckCollision(solids))
			Location.y -= MovementSpeed;
	}
	if (currentKeyStates[SDL_SCANCODE_A])
	{
		if(!currentKeyStates[SDL_SCANCODE_W] && 
			!currentKeyStates[SDL_SCANCODE_S])
			Rotation = 0.0;
		MovingDirection = Direction::Left;
		Location.x -= MovementSpeed;
		if (checkCollision && CheckCollision(solids))
			Location.x += MovementSpeed;

	}
	if (currentKeyStates[SDL_SCANCODE_D])
	{
		if (!currentKeyStates[SDL_SCANCODE_W] && 
			!currentKeyStates[SDL_SCANCODE_S])
			Rotation = 0.0;
		MovingDirection = Direction::Right;
		Location.x += MovementSpeed;
		if (checkCollision && CheckCollision(solids))
			Location.x -= MovementSpeed;
	}

	AddToTrail();

	//Bullets
	Direction shootDirection = Direction::NoDirection;
	if (currentKeyStates[SDL_SCANCODE_UP])
	{	
		MovingDirection = Direction::Up;
		if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			shootDirection = Direction::UpRight;
			Rotation = 45.0;
		}
		else if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			shootDirection = Direction::UpLeft;
			Rotation = -45.0;
		}
		else
		{
			shootDirection = Direction::Up;
			Rotation = 0.0;
		}
	}
	else if (currentKeyStates[SDL_SCANCODE_DOWN])
	{
		MovingDirection = Direction::Down;
		if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			shootDirection = Direction::DownRight;
			Rotation = -45.0;
		}
		else if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			shootDirection = Direction::DownLeft;
			Rotation = 45.0;
		}
		else
		{
			shootDirection = Direction::Down;
			Rotation = 0.0;
		}
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT])
	{
		Rotation = 0.0;
		MovingDirection = Direction::Right;
		shootDirection = Direction::Right;
	}
	else if (currentKeyStates[SDL_SCANCODE_LEFT])
	{
		Rotation = 0.0;
		MovingDirection = Direction::Left;
		shootDirection = Direction::Left;
	}

	if (!BulletTimer.IsStarted() && shootDirection != Direction::NoDirection)
	{
		Projectiles.FireProjectile(&Location, shootDirection);
		BulletTimer.Start();
	}

	if (BulletTimer.IsStarted())
	{
		if (BulletTimer.GetTicks() > RateOfFire)
			BulletTimer.Stop();
	}
	MoveFrame(MovingDirection);
	Projectiles.Update(camera, solids);
}

void Player::ControllerInput()
{
	//to ignore light presses
	const int JOYSTICK_DEAD_ZONE = 8000;
	SDL_Joystick* gGameController = NULL; 


}

void Player::AddToTrail()
{
	if (Trail[0].x != Location.x && Trail[0].y != Location.y)
	{
		for (int i = MAXTRAIL - 1; i > 0; i--)
		{
			Trail[i] = Trail[i - 1];
		}
		Trail[0] = SDL_Point{ Location.x, Location.y };
	}
}

void Player::Reset()
{
	HitPoints = StartingHitPoints; 
	AnimatedObject::Reset();
}

vector<SDL_Point>* Player::GetTrail()
{
	return &Trail;
}