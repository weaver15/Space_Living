#include "Door.h"

Door::Door()
{
	SetTag("Door");
}


Door::~Door()
{
}

Door::Door(int id, SDL_Rect textureInfo, SDL_Rect location, bool isVertical, bool isBossDoor)
	:AnimatedObject(textureInfo, location)
{
	Id = id;
	IsVertical = isVertical;
	IsBossDoor = isBossDoor; 
	InteractionArea = { location.x - 10, location.y - 10, location.w  + 20, location.h  + 20};
	SetIsAlive(true);
	SetTag("Door");
}



void Door::CloseDoor(SDL_Rect* texture)
{
	IsOpen = false;
	ChangeTextureLocation(*texture);
}

void Door::OpenDoor(SDL_Rect* texture)
{
	IsOpen = true;
	ChangeTextureLocation(*texture);
}

DoorManager::DoorManager()
{
	Doors.resize(0);
}
DoorManager::~DoorManager()
{
	NumberOfDoors = 0;
	Doors.resize(0);
}
DoorManager::DoorManager(int numberOfDoors)
{
	Doors.resize(0);
	NumberOfDoors = numberOfDoors;
}

void DoorManager::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	for (auto& door : Doors)
	{
		if (door.IsAlive() && (SDL_HasIntersection(door.GetLocation(), camera)))
			door.Draw(renderer, texture, camera);
	}
	if (BossDoor.IsAlive() && (SDL_HasIntersection(BossDoor.GetLocation(), camera)))
		BossDoor.Draw(renderer, texture, camera);
}

void DoorManager::AddDoor(Door door)
{
	if (door.IsBossDoor)
		BossDoor = door;
	else
		Doors.push_back(door);
}

int DoorManager::CheckCollision(SDL_Rect* dimensions)
{
	for (size_t index = 0; index < Doors.size(); index++)
	{
		if (CheckCollision(dimensions, index))
			return index;
	}
	return -1;
}
bool DoorManager::CheckCollision(SDL_Rect* dimensions, int index)
{
	if (Doors[index].IsAlive() && !Doors[index].IsOpen &&
		SDL_HasIntersection(&Doors[index].InteractionArea, dimensions))
		return true;
	else
		return false;
}

int DoorManager::OpenDoor(SDL_Rect* dimensions, const int playerEnergy)
{
	int index = CheckCollision(dimensions);
	if (index >= 0)
	{
		int cost = Doors[index].OpenCost;
		if (playerEnergy >= cost)
		{
			if (Doors[index].IsVertical)
				Doors[index].OpenDoor(&OpenDoorVerticalTextureLocation);
			else
				Doors[index].OpenDoor(&OpenDoorHorizontalTextureLocation);
			return -cost;
		}
	}
	return 0;
}

bool DoorManager::OpenBossDoor(SDL_Rect* dimensions, bool hasKey)
{
	if (hasKey && BossDoor.IsAlive() && SDL_HasIntersection(&BossDoor.InteractionArea, dimensions))
	{
		if (BossDoor.IsVertical)
			BossDoor.OpenDoor(&OpenDoorVerticalTextureLocation);
		else
			BossDoor.OpenDoor(&OpenDoorHorizontalTextureLocation);
		return true;
	}
	return false;
}

std::vector<Door> DoorManager::GetDoors()
{
	std::vector<Door> doors = Doors;
	doors.push_back(BossDoor);
	return doors; 
}

Door DoorManager::GetDoor(int index)
{
	return Doors[index];
}

void DoorManager::SetTextureLocations(SDL_Rect vertClosedDoor, SDL_Rect vertOpenDoor, SDL_Rect horzClosedDoor, SDL_Rect horzOpenDoor)
{
	ClosedDoorVerticalTextureLocation = vertClosedDoor;
	OpenDoorVerticalTextureLocation = vertOpenDoor;
	ClosedDoorHorizontalTextureLocation = horzClosedDoor;
	OpenDoorHorizontalTextureLocation = horzOpenDoor;
}

void DoorManager::ResetDoors()
{
	for (auto& door : Doors)
	{
		if (door.IsVertical)
			door.CloseDoor(&ClosedDoorVerticalTextureLocation);
		else
			door.CloseDoor(&ClosedDoorHorizontalTextureLocation);
	}
}
std::vector<SDL_Rect*> DoorManager::GetDoorLocations()
{
	std::vector<SDL_Rect*> result;
	for (auto& door : Doors)
	{
		if(door.IsAlive() && !door.IsOpen)
			result.push_back(door.GetLocation());
	}
	return result; 
}

bool DoorManager::IsOpen(int id)
{
	for (auto& door : Doors)
	{
		if (door.Id == id)
		{
			return door.IsOpen;
		}
	}
	return false;
}