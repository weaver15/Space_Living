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
	Impassable = 1,
	WallLocation_None = 2,
	WallLocation_Top = 3,
	WallLocation_Right = 4,
	WallLocation_Bottom = 5,
	WallLocation_Left = 6,
	WallLocation_TopRight = 7,
	WallLocation_BottomRight = 8,
	WallLocation_BottomLeft = 9,
	WallLocation_TopLeft = 10,
	WallLocation_TopBottom = 11,
	WallLocation_LeftRight = 12,
	WallLocation_LeftRightBottom = 13,
	WallLocation_LeftRightTop = 14,
	WallLocation_RightTopBottom = 15,
	WallLocation_LeftTopBottom = 16,

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

	SDL_Rect GetLeftWall(SDL_Rect location);
	SDL_Rect GetRightWall(SDL_Rect location);
	SDL_Rect GetTopWall(SDL_Rect location);
	SDL_Rect GetBottomWall(SDL_Rect location);
};