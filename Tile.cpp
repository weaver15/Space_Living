#include "Tile.h"



Tile::Tile()
{
}

Tile::Tile(TileDetails details) : AnimatedObject(details.TextureLocation, details.TileLocation)
{
	iTileType = details.TileType;
	WallLocation = details.TileWalls;
	GridLoc = details.GridLoc; 
	SetWalls();
	SetIsAlive(true);
}

Tile::~Tile()
{
}


void Tile::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	if (SDL_HasIntersection(GetLocation(), camera))
	{
		AnimatedObject::Draw(renderer, texture, camera);
	}
}

void Tile::SetWalls()
{

	if (WallLocation == WallLocation::WallLocation_Left)
	{
		SDL_Rect wall = *GetLocation();
		wall.w -= 145;
		vecWalls.push_back(wall);
	}
	else if (WallLocation == WallLocation::WallLocation_Right)
	{
		SDL_Rect wall = *GetLocation();
		wall.w -= 145;
		wall.x += 145; 
		vecWalls.push_back(wall);
	}
	else if (WallLocation == WallLocation::WallLocation_Top)
	{
		SDL_Rect wall = *GetLocation();
		wall.h -= 145;
		vecWalls.push_back(wall);
	}
	else if (WallLocation == WallLocation::WallLocation_Bottom)
	{
		SDL_Rect wall = *GetLocation();
		wall.h -= 145;
		wall.y += 145;
		vecWalls.push_back(wall);
	}
	else if (WallLocation == WallLocation::WallLocation_TopRight)
	{
		SDL_Rect top = *GetLocation();
		top.h -= 145;
		vecWalls.push_back(top);

		SDL_Rect right = *GetLocation();
		right.w -= 145;
		right.x += 145;
		vecWalls.push_back(right);

	}
	else if (WallLocation == WallLocation::WallLocation_BottomRight)
	{
		SDL_Rect bottom = *GetLocation();
		bottom.h -= 145;
		bottom.y += 145;
		vecWalls.push_back(bottom);

		SDL_Rect right = *GetLocation();
		right.w -= 145;
		right.x += 145;
		vecWalls.push_back(right);

	}
	else if (WallLocation == WallLocation::WallLocation_BottomLeft)
	{
		SDL_Rect bottom = *GetLocation();
		bottom.h -= 145;
		bottom.y += 145;
		vecWalls.push_back(bottom);

		SDL_Rect left = *GetLocation();
		left.w -= 145;
		vecWalls.push_back(left);
	}
	else if (WallLocation == WallLocation::WallLocation_TopLeft)
	{
		SDL_Rect top = *GetLocation();
		top.h -= 145;
		vecWalls.push_back(top);

		SDL_Rect left = *GetLocation();
		left.w -= 145;
		vecWalls.push_back(left);
	}
	else if (WallLocation == WallLocation::WallLocation_TopBottom)
	{
		SDL_Rect top = *GetLocation();
		top.h -= 145;
		vecWalls.push_back(top);

		SDL_Rect bottom = *GetLocation();
		bottom.h -= 145;
		bottom.y += 145;
		vecWalls.push_back(bottom);
	}
	else if (WallLocation == WallLocation::WallLocation_LeftRight)
	{
		SDL_Rect left = *GetLocation();
		left.w -= 145;
		vecWalls.push_back(left);

		SDL_Rect right = *GetLocation();
		right.w -= 145;
		right.x += 145;
		vecWalls.push_back(right);
	}
}

SDL_Point Tile::GetTileLocation()
{
	auto location = GetLocation();
	SDL_Point result = { location->x + (location->w / 2), location->y + (location->h / 2) };
	return result;
}