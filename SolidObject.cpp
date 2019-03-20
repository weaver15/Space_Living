#include "SolidObject.h"


#pragma region Solid Object
SolidObject::SolidObject()
{
}

SolidObject::SolidObject(SDL_Rect textureInfo, SDL_Rect location)
	:AnimatedObject(textureInfo, location)
{
	SetIsAlive(true);
}

SolidObject::~SolidObject()
{
}

#pragma endregion

#pragma region Solid Object Manager
SolidObjectManager::SolidObjectManager()
{
}
SolidObjectManager::SolidObjectManager(vector<pair<SDL_Rect, SDL_Rect>> objects)
{
	for each (pair<SDL_Rect, SDL_Rect> solid in objects)
	{
		AddObject(solid.first, solid.second);
	}
}


SolidObjectManager::~SolidObjectManager()
{
}


void SolidObjectManager::AddObject(SDL_Rect textureInfo, SDL_Rect location)
{
	SolidObjects.push_back(SolidObject(textureInfo, location));
}

bool SolidObjectManager::CheckCollision(SDL_Rect* dimensions)
{
	for (size_t index = 0; index < SolidObjects.size(); index++)
	{
		if (SDL_HasIntersection(SolidObjects[index].GetLocation(), dimensions))
			return true;
	}
	return false;
}

void SolidObjectManager::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	for (int index = 0; index < SolidObjects.size(); ++index)
	{
		if (SDL_HasIntersection(SolidObjects[index].GetLocation(), camera))
		{
			SolidObjects[index].Draw(renderer, texture, camera);
		}
	}
}

std::vector<SDL_Rect> SolidObjectManager::GetSolidObjectsOnScreen(SDL_Rect camera)
{
	std::vector<SDL_Rect> solids;
	for each (SolidObject solid in SolidObjects)
	{
		if (SDL_HasIntersection(solid.GetLocation(), &camera))
		{
			solids.push_back(*solid.GetLocation());
		}
	}
	return solids;
}

#pragma endregion