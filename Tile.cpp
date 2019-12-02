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
	SDL_Rect location = *GetLocation();
	if (WallLocation == WallLocation::WallLocation_Left)
	{
		vecWalls.push_back(GetLeftWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_Right)
	{
		vecWalls.push_back(GetRightWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_Top)
	{
		vecWalls.push_back(GetTopWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_Bottom)
	{
		vecWalls.push_back(GetBottomWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_TopRight)
	{
		vecWalls.push_back(GetTopWall(location));
		vecWalls.push_back(GetRightWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_BottomRight)
	{
		vecWalls.push_back(GetBottomWall(location));
		vecWalls.push_back(GetRightWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_BottomLeft)
	{
		vecWalls.push_back(GetBottomWall(location));
		vecWalls.push_back(GetLeftWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_TopLeft)
	{
		vecWalls.push_back(GetTopWall(location));
		vecWalls.push_back(GetLeftWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_TopBottom)
	{
		vecWalls.push_back(GetTopWall(location));
		vecWalls.push_back(GetBottomWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_LeftRight)
	{
		vecWalls.push_back(GetLeftWall(location));
		vecWalls.push_back(GetRightWall(location));
	}

	else if (WallLocation == WallLocation::WallLocation_LeftRightBottom)
	{
		vecWalls.push_back(GetLeftWall(location));
		vecWalls.push_back(GetRightWall(location));
		vecWalls.push_back(GetBottomWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_LeftRightTop)
	{
		vecWalls.push_back(GetLeftWall(location));
		vecWalls.push_back(GetRightWall(location));
		vecWalls.push_back(GetTopWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_RightTopBottom)
	{
		vecWalls.push_back(GetRightWall(location));
		vecWalls.push_back(GetTopWall(location));
		vecWalls.push_back(GetBottomWall(location));
	}
	else if (WallLocation == WallLocation::WallLocation_LeftTopBottom)
	{
		vecWalls.push_back(GetLeftWall(location));
		vecWalls.push_back(GetTopWall(location));
		vecWalls.push_back(GetBottomWall(location));
	}
}

SDL_Rect Tile::GetLeftWall(SDL_Rect location)
{
	location.w -= 145;
	return location;
}
SDL_Rect Tile::GetRightWall(SDL_Rect location)
{
	location.w -= 145;
	location.x += 145;
	return location;
}
SDL_Rect Tile::GetTopWall(SDL_Rect location)
{
	location.h -= 145;
	return location;
}
SDL_Rect Tile::GetBottomWall(SDL_Rect location)
{
	location.h -= 145;
	location.y += 145;
	return location;
}

SDL_Point Tile::GetTileLocation()
{
	auto location = GetLocation();
	SDL_Point result = { location->x + (location->w / 2), location->y + (location->h / 2) };
	return result;
}