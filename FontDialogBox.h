#pragma once
#include <SDL.h>
#include "AnimatedObject.h"
#include "Font.h"

enum FontDialogBoxTag
{
	Stat,
	DoorCost
};
static const std::string FontDialogBoxTagStrings[] = { "Stat", "DoorCost"};

struct LocationDialogBox
{
	int Id;
	SDL_Point Location;
	string Value;
	bool HasDisplayed; 
};


class FontDialogBox : public AnimatedObject
{
public:
	FontDialogBox();
	~FontDialogBox();
	FontDialogBox(std::pair<SDL_Rect, SDL_Rect> details);

	void Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera);
	bool InitializeFont(SDL_Renderer* renderer, int fontSize = 22, FontDialogBoxTag tag = FontDialogBoxTag::Stat);
	void UpdateFont(SDL_Renderer* renderer, std::string value, FontDialogBoxTag tag = FontDialogBoxTag::Stat);
	void UpdateLocation(SDL_Rect* camera, FontDialogBoxTag tag);
private:
	Font _Font;

};