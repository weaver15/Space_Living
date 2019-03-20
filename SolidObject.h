#pragma once
#include "AnimatedObject.h"
#include <vector>

class SolidObject : public AnimatedObject
{
public:
	SolidObject();
	SolidObject(SDL_Rect textureInfo, SDL_Rect location);

	~SolidObject();

private:
	bool IsMoveable; 
};

class SolidObjectManager
{
public:
	SolidObjectManager();
	SolidObjectManager(vector<pair<SDL_Rect, SDL_Rect>> objects);
	~SolidObjectManager();

	void AddObject(SDL_Rect textureInfo, SDL_Rect location);
	bool CheckCollision(SDL_Rect* dimension);
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	std::vector<SDL_Rect> SolidObjectManager::GetSolidObjectsOnScreen(SDL_Rect camera);
private:
	std::vector<SolidObject> SolidObjects;
	std::vector<SDL_Rect> SolidObjectTextures;
};

