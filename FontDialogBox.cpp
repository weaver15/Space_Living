#include "FontDialogBox.h"

#pragma region Font Dialog Box

#pragma region Constructor/Deconstructor
FontDialogBox::FontDialogBox()
{
	SetTag("FontDialogBox");
}
FontDialogBox::~FontDialogBox()
{

}
FontDialogBox::FontDialogBox(std::pair<SDL_Rect, SDL_Rect> details)
	:AnimatedObject(details.first, details.second)
{
	SetTag("FontDialogBox");
}
#pragma endregion 


void FontDialogBox::Draw(SDL_Renderer* renderer, Texture* texture, SDL_Rect* camera)
{
	AnimatedObject::Draw(renderer, texture, camera);
	if (this->IsAlive())
		_Font.RenderText(renderer, camera);
}
bool FontDialogBox::InitializeFont(SDL_Renderer* renderer, int fontSize, FontDialogBoxTag tag)
{
	_Font.LoadFont(renderer, fontSize);
	SDL_Color textColor = { 0, 0, 0 };
	SDL_Rect location = { 0, 0, 0 };
	_Font.AddText(renderer, "0", &location, FontDialogBoxTagStrings[tag], textColor);

	return true;
}

void FontDialogBox::UpdateFont(SDL_Renderer* renderer, std::string value, FontDialogBoxTag tag)
{
	_Font.UpdateText(renderer, value, FontDialogBoxTagStrings[tag]);
}

void FontDialogBox::UpdateLocation(SDL_Rect* location, FontDialogBoxTag tag)
{
	SetLocation(location);
	_Font.UpdateFontLocation(location, FontDialogBoxTagStrings[tag]);
}
#pragma endregion