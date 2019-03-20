#pragma once
#include "DroppedObject.h"

#pragma region Dropped Object

DroppedObject::DroppedObject()
{
	SetTag("DroppedObject");
	ObjectType = DroppedObjectType::None;
}
DroppedObject::~DroppedObject()
{
	ObjectType = DroppedObjectType::None;
	AnimatedObject::~AnimatedObject(); 
}

DroppedObject::DroppedObject(SDL_Rect textureInfo, SDL_Rect location, DroppedObjectType type)
	:AnimatedObject(textureInfo, location)
{
	SetTag("DroppedObject");
	SetIsAlive(true);
	ObjectType = type;
}

#pragma endregion

#pragma region Dropped Object Manager
DroppedObjectManager::DroppedObjectManager()
{
	DroppedObjects.resize(1);
}
DroppedObjectManager::~DroppedObjectManager()
{
	//for each object ~ it? 
	DroppedObjects.resize(1);
	MaxNumberOfDroppedObjects = 0;
	AliveDroppedObjects = 0;
}
DroppedObjectManager::DroppedObjectManager(int maxNumberOfDroppedObjects)
{
	DroppedObjects.resize(maxNumberOfDroppedObjects);
	MaxNumberOfDroppedObjects = maxNumberOfDroppedObjects;
}

void DroppedObjectManager::AddDroppedObject(SDL_Rect location, DroppedObjectType type)
{
	//Add to pick up objects, defaults texture location
	location.h = TextureLocation.h;
	location.w = TextureLocation.w;
	AddDroppedObject(DroppedObject(TextureLocation, location, type));
}
void DroppedObjectManager::AddDroppedObject(DroppedObject obj)
{
	//Add to pick up objects
	//Doesn't construct object, used for boss key and other
	//non standard dropped objects

	if (AliveDroppedObjects < MaxNumberOfDroppedObjects)
	{
		int index = FirstDeadDroppedObject();
		DroppedObjects[index] = obj;
		AliveDroppedObjects = AliveDroppedObjects + 1;
	}

}
DroppedObjectType DroppedObjectManager::CheckCollision(SDL_Rect* playerLocation)
{
	if (AliveDroppedObjects > 0)
	{
		for (size_t i = 0; i < DroppedObjects.size(); i++)
		{
			if (DroppedObjects[i].CheckCollision(playerLocation))
			{
				auto type = DroppedObjects[i].ObjectType;
				RemoveDroppedObject(i);
				return type;
			}
		}
	}
	return DroppedObjectType::None;
}
void DroppedObjectManager::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	if (AliveDroppedObjects > 0)
	{
		for (int index = 0; index < MaxNumberOfDroppedObjects; ++index)
		{
			if (DroppedObjects[index].IsAlive())
			{
				DroppedObjects[index].Draw(renderer, texture, camera);
			}
		}
	}
}
int DroppedObjectManager::FirstDeadDroppedObject() const
{
	for (int i = 0; i < DroppedObjects.size(); i++)
	{
		if (!DroppedObjects[i].IsAlive())
			return i;
	}
}
int DroppedObjectManager::GetNumberOfDroppedObjects() const
{
	return AliveDroppedObjects;
}
void DroppedObjectManager::RemoveDroppedObject(int index)
{
	if (AliveDroppedObjects > 0)
	{
		DroppedObjects[index].SetIsAlive(false);
		AliveDroppedObjects = AliveDroppedObjects - 1;
	}

}
void DroppedObjectManager::SetTextureLocation(SDL_Rect texture)
{
	TextureLocation = texture;
}

void DroppedObjectManager::Reset()
{
	for (auto object : DroppedObjects)
	{
		object.SetIsAlive(false);
		AliveDroppedObjects = AliveDroppedObjects - 1;
	}
}

#pragma endregion