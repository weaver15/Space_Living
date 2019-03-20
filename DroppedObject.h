#pragma once
#include "AnimatedObject.h"
#include <vector>

enum DroppedObjectType
{
	Ammo,
	Health,
	Item,
	Key,
	None
};

class DroppedObject : public AnimatedObject
{
public:
	DroppedObject();
	~DroppedObject();
	DroppedObject(SDL_Rect textureInfo, SDL_Rect location, DroppedObjectType type);
	DroppedObjectType ObjectType;
};

class DroppedObjectManager
{
public:
	DroppedObjectManager();
	~DroppedObjectManager();
	DroppedObjectManager(int maxNumberOfDroppedObjects);

	void AddDroppedObject(SDL_Rect location, DroppedObjectType type);
	void AddDroppedObject(DroppedObject obj);
	DroppedObjectType CheckCollision(SDL_Rect* playerLocation);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	int GetNumberOfDroppedObjects() const;
	void RemoveDroppedObject(int index);
	void SetTextureLocation(SDL_Rect texture);
	void Reset();

private:
	int AliveDroppedObjects = 0;
	std::vector<DroppedObject> DroppedObjects;
	int MaxNumberOfDroppedObjects = 0;
	SDL_Rect TextureLocation;

	int FirstDeadDroppedObject() const;
};
