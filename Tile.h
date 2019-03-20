#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <vector>
#include "AnimatedObject.h"
#include <tuple>

typedef std::tuple<int, int> GridLocation;

enum WallLocation
{
	Impassable,
	WallLocation_None,
	WallLocation_Top,
	WallLocation_Right,
	WallLocation_Bottom,
	WallLocation_Left,
	WallLocation_TopRight,
	WallLocation_BottomRight,
	WallLocation_BottomLeft,
	WallLocation_TopLeft,
	WallLocation_TopBottom,
	WallLocation_LeftRight
};

struct TileDetails
{
	SDL_Rect TextureLocation;
	SDL_Rect TileLocation;
	int TileType; 
	WallLocation TileWalls;
	GridLocation GridLoc; 
};


class Tile : public AnimatedObject
{
public:
	Tile();
	Tile(TileDetails details);
	~Tile();
	
	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	int GetTileType() const { return iTileType; };
	WallLocation GetWallLocation() const { return WallLocation; };
	GridLocation GetGridLocation() const { return GridLoc; };
	std::vector<SDL_Rect> GetWalls() { return vecWalls; };
	SDL_Point GetTileLocation();

private:
	int iTileType; 
	WallLocation WallLocation; 
	std::vector<SDL_Rect> vecWalls;
	GridLocation GridLoc;
	void SetWalls();
};