#pragma once
#include "AnimatedObject.h"


#pragma region Constructor/Deconstructor
AnimatedObject::AnimatedObject()
{
}
AnimatedObject::AnimatedObject(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol)
{
	TotalRowCol= totalRowCol;
	StartLocation = { location.x, location.y };
	TextureInformation = textureInfo;
	Location = location;
	ObjectTexture.SetSize(location.w, location.h);
}

AnimatedObject::~AnimatedObject()
{
	Alive = false;
	Location = TextureInformation = SDL_Rect();
	TotalRowCol = StartLocation = CurrentRowCol = SDL_Point();
	ObjectTexture.~Texture();
}
#pragma endregion

#pragma region Draw

inline void AnimatedObject::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera, double rotation, double scale)
{
	//Draw for player
	if (Alive)
	{
		SDL_Rect* currentClip = new SDL_Rect();
		currentClip->w = TextureInformation.w;
		currentClip->h = TextureInformation.h;
		currentClip->x = TextureInformation.x + (Location.w * (int)(CurrentRowCol.x / TotalRowCol.x));
		currentClip->y = TextureInformation.y + (Location.h * CurrentRowCol.y);
		texture->Render(Location.x - camera->x, Location.y - camera->y, renderer, currentClip, rotation, NULL, SDL_FLIP_NONE, Tag, scale);
	}
}

inline void AnimatedObject::Draw(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture, double rotation)
{
	//Draw that uses a SDL_Texture, for stretched textures
	if (Alive)
	{
		SDL_Rect* currentClip = new SDL_Rect();
		currentClip->w = TextureInformation.w;
		currentClip->h = TextureInformation.h;
		currentClip->x = TextureInformation.x + (Location.w * (int)(CurrentRowCol.x / TotalRowCol.x));
		currentClip->y = TextureInformation.y + (Location.h * CurrentRowCol.y);
		SDL_Rect renderQuad = { Location.x - x, Location.y - y, Location.w, Location.h };
		SDL_RenderCopyEx(renderer, texture, currentClip, &renderQuad, rotation, NULL, SDL_FLIP_NONE);
	}
}
#pragma endregion

#pragma region Texture
bool AnimatedObject::LoadTexture(std::string path, SDL_Renderer* renderer, SDL_Window* gWindow)
{
	bool TextureLoaded = ObjectTexture.LoadTexture(path, renderer, gWindow);
	return TextureLoaded;
}

void AnimatedObject::ChangeTextureLocation(SDL_Rect rect)
{
	TextureInformation = rect;
}
#pragma endregion

void AnimatedObject::Reset()
{
	SetIsAlive(true);
	Location.x = StartLocation.x;
	Location.y = StartLocation.y;

}

void AnimatedObject::MoveFrame(int row)
{
	//Increment the current sprite frame
	CurrentRowCol.y = row;
}

void AnimatedObject::UpdateLocation(int direction, int movementSpeed)
{
	if (direction == Direction::Up)
		Location.y -= movementSpeed;
	if (direction == Direction::Down)
		Location.y += movementSpeed;
	if (direction == Direction::Left)
		Location.x -= movementSpeed;
	if (direction == Direction::Right)
		Location.x += movementSpeed;
}

 inline SDL_Rect* AnimatedObject::GetLocation()
{
	return &Location;
}

SDL_Point AnimatedObject::GetTileLocation(const int tileSize) const
{
	return SDL_Point{ (int)(Location.x / tileSize) , (int)(Location.y / tileSize) };
}

void AnimatedObject::SetLocation(int x, int y)
{
	Location.y = y;
	Location.x = x;
}

void AnimatedObject::SetLocation(SDL_Rect* rect, SDL_Point* slot)
{
	if(slot != nullptr)
		Location = SDL_Rect{ rect->x + slot->x, rect->y + slot->y, rect->w, rect->h };
	else
		Location = *rect;
}

#pragma region Alive
bool AnimatedObject::IsAlive() const
{
	return Alive;
}

void AnimatedObject::SetIsAlive(bool isAlive)
{
	Alive = isAlive;
}
#pragma endregion

#pragma region Collision
bool AnimatedObject::CheckCollision(SDL_Rect* rect) const
{
	if (Alive)
	{
		if (SDL_HasIntersection(&Location, rect))
			return true;
	}
	return false;
}

bool AnimatedObject::CheckCollision(std::vector<SDL_Rect> solids) const
{
	for each (SDL_Rect rec in solids)
	{
		if (CheckCollision(&rec))
			return true;
	}
	return false;
}
#pragma endregion

inline bool AnimatedObject::IsOnCamera(SDL_Rect* camera) const
{
	return SDL_HasIntersection(&Location, camera);
}

#pragma region Tag
void AnimatedObject::SetTag(std::string tag)
{
	Tag = tag;
}

std::string AnimatedObject::GetTag() const
{
	return Tag;
}
#pragma endregion