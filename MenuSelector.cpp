#include "MenuSelector.h"

#pragma region Constructor/Deconstructor
MenuSelector::MenuSelector()
{
	SetTag("MenuSelector");
}
MenuSelector::~MenuSelector()
{
	RowCol = SDL_Point();
}
MenuSelector::MenuSelector(SDL_Rect textureInfo, SDL_Rect location, SDL_Point totalRowCol, SDL_Point gridSize)
	:AnimatedObject(textureInfo, location, totalRowCol)
{
	GridSize = gridSize;
	SetTag("MenuSelector");
	SetIsAlive(true);
}
#pragma endregion 

void MenuSelector::MoveSelector(Direction direction)
{
	switch (direction)
	{
	case Down:
		if (RowCol.y < GridSize.y - 1)
		{
			RowCol.y++;
			Location.y = Location.y + Location.h + DrawOffset;
		}
		break;
	case Left:
		if (RowCol.x > 0)
		{
			RowCol.x--;
			Location.x = Location.x - Location.w - DrawOffset;
		}
		break;
	case Right:
		if (RowCol.x < GridSize.x - 1)
		{
			RowCol.x++;
			Location.x = Location.x + Location.w + DrawOffset;
		}
		break;
	case Up:
		if (RowCol.y > 0)
		{
			RowCol.y--;
			Location.y = Location.y - Location.h - DrawOffset;
		}
		break;
	default:
		break;
	}
}

SDL_Point MenuSelector::GetSelectedSlot()
{
	return RowCol;
}

void MenuSelector::SetLocation(SDL_Rect* camera)
{
	AnimatedObject::SetLocation((camera->x + camera->w / 4) + ((RowCol.x) * (Location.w + DrawOffset)) + 50,
		(camera->y + camera->h / 4) + ((RowCol.y) * (Location.h + DrawOffset) + 75));
}

void MenuSelector::SetDrawOffset(int offset)
{
	DrawOffset = offset; 
}