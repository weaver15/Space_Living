#pragma once
#include "Texture.h"
#include "Enums.cpp"
#include "Calculate.h"
#include <vector>

using namespace std; 
enum Direction
{
	Down,
	Left,
	Right,
	Up,
	UpLeft,
	UpRight,
	DownLeft,
	DownRight,
	NoDirection
};

class AnimatedObject
{
public:
	AnimatedObject();
	AnimatedObject(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol = {1,1});
	~AnimatedObject();

	virtual void ChangeTextureLocation(SDL_Rect rect);
	virtual bool CheckCollision(SDL_Rect* rect) const;
	virtual bool CheckCollision(std::vector<SDL_Rect> solids) const;
	virtual void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect * camera, double rotation = 0);
	virtual void Draw(int x, int y, SDL_Renderer* renderer, SDL_Texture* texture = NULL, double rotation = 0);
	virtual SDL_Rect* GetLocation();
	virtual std::string GetTag() const;	
	virtual bool IsAlive() const;
	virtual bool LoadTexture(std::string path, SDL_Renderer* renderer, SDL_Window* gWindow);
	virtual void MoveFrame(int row);
	virtual void Reset();
	virtual void SetLocation(int x, int y);
	virtual void SetLocation(SDL_Rect* rect, SDL_Point* slot = nullptr);
	virtual void SetIsAlive(bool isAlive);
	virtual void SetTag(std::string tag);
	virtual void UpdateLocation(int direction, int movementSpeed);
	virtual bool IsOnCamera(SDL_Rect* camera) const;
	virtual SDL_Point GetTileLocation(const int tileSize) const;

protected:
	SDL_Rect TextureInformation;
	Texture ObjectTexture;
	SDL_Point StartLocation;
	SDL_Point CurrentRowCol = { 0,0 };
	SDL_Point TotalRowCol;
	SDL_Rect Location;

private:
	bool Alive = false;
	std::string Tag = "Object"; 
};

