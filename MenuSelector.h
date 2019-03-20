#pragma once
#include "AnimatedObject.h"

class MenuSelector : public AnimatedObject
{
public: 

	MenuSelector();
	~MenuSelector();
	MenuSelector(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol, SDL_Point gridSize);
	
	SDL_Point GetSelectedSlot();
	void SetLocation(SDL_Rect* camera);
	void MoveSelector(Direction direction); 
	void SetDrawOffset(int offset);

	int GetRow() const { return RowCol.y; };
private:
	SDL_Point RowCol = {0,0};
	SDL_Point GridSize = { 0,0 };
	int DrawOffset = 6;
};

