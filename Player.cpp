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
	EnergyCount = 15; 
	Trail.resize(MAXTRAIL);
	SetTag("Player");
}

Player::~Player()
{
	Projectiles.~ProjectileManager();
	BulletTimer.~Timer();
	PlayerInventory.~Inventory();
	RateOfFire = 0;
	MovementSpeed = 0;
}

void Player::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera)
{
	Projectiles.Draw(renderer, texture, camera);
	AnimatedObject::Draw(renderer, texture, camera, Rotation);
}
void Player::DrawInventory(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera)
{
	if(PlayerInventory.IsInventoryShowing())
		PlayerInventory.Draw(renderer, texture, camera);
}

void Player::Update(SDL_Rect * camera, bool checkCollision, std::vector<SDL_Rect> solids)
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	//Left Shift to run
	if (!PlayerInventory.IsInventoryShowing())
	{
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
	else
	{
		PlayerInventory.Update();
	}
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

void Player::PlayerInventoryInput(SDL_Renderer* renderer, SDL_Event* e)
{
	SDL_Keycode keyCode = e->key.keysym.sym;
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
	{
		if (keyCode == SDLK_w)
			PlayerInventory.MoveSelector(Direction::Up, renderer);
		if (keyCode == SDLK_s)
			PlayerInventory.MoveSelector(Direction::Down, renderer);
		if (keyCode == SDLK_a)
			PlayerInventory.MoveSelector(Direction::Left, renderer);
		if (keyCode == SDLK_d)
			PlayerInventory.MoveSelector(Direction::Right, renderer);

		if (keyCode == SDLK_RETURN)
		{
			bool wasWeaponEquiped = PlayerInventory.EquipSelectedItem(renderer, &PlayerStats);
			if (wasWeaponEquiped)
				Projectiles.SetDamage(PlayerStats.iAttack);
		}
			
	}
}

void Player::InitializeInventory(InventoryInformation inventoryInfo, EquipmentSlotInformation equipInfo)
{
	inventoryInfo.TotalRowCol = { 1,1 };
	inventoryInfo.Location = Location; 
	PlayerInventory = Inventory(inventoryInfo);
	PlayerInventory.SetEquipmentSlots(equipInfo);
	PlayerInventory.InitializeStatTextBox(inventoryInfo.TextBoxTextureInfo);
}
void Player::InitializeInventoryFont(SDL_Renderer* renderer)
{
	PlayerInventory.InitializeFont(renderer);
}

void Player::ToggleInventory(SDL_Renderer* renderer, SDL_Rect * camera)
{
	if (!PlayerInventory.IsInventoryShowing())
	{
		PlayerInventory.UpdateInventoryLocation(renderer, camera);
		PlayerInventory.ShowInventory();
	}
	else
		PlayerInventory.HideInventory();
}

void Player::PickUpItem(InventoryItem* item)
{
	PlayerInventory.AddItem(item);
}

void Player::IncrementEnergyCount(int value)
{
	EnergyCount += value; 
}

const int Player::GetEnergyCount()
{
	return EnergyCount;
}

void Player::Reset()
{
	HitPoints = StartingHitPoints; 
	EnergyCount = 0;
	AnimatedObject::Reset();
}

vector<SDL_Point>* Player::GetTrail()
{
	return &Trail;
}

const bool Player::GetHasBossKey()
{
	return HasBossDoorKey; 
}

void Player::SetHasBossKey(bool hasKey)
{
	HasBossDoorKey = hasKey; 
}