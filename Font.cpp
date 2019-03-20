#include "Font.h"
Font::Font()
{
}


Font::~Font()
{
}

void Font::Free()
{

}

void Font::LoadFont(SDL_Renderer* renderer, int fontSize)
{
	_Font = TTF_OpenFont("Texture/font.TTF", fontSize);
}

bool Font::AddText(SDL_Renderer* renderer, std::string textureText,  SDL_Rect* location, std::string tag, SDL_Color textColor)
{
	//TODO: don't let there be two with the same tag

	SDL_Surface* textSurface = TTF_RenderText_Solid(_Font, textureText.c_str(), textColor);
	Text newText;
	newText.Value = textureText; 
	newText.Tag = tag; 
	newText.Location = *(location);
	newText.Texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	newText.Location.w = textSurface->w;
	newText.Location.h = textSurface->h;
	SDL_FreeSurface(textSurface);	
	_Text.push_back(newText);

	return newText.Texture != NULL;
}

void Font::RenderText(SDL_Renderer* renderer)
{
	//Render to screen
	for (auto text : _Text)
	{
		SDL_RenderCopyEx(renderer, text.Texture, NULL, &text.Location, NULL, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	}
	
}
void Font::RenderText(SDL_Renderer* renderer, SDL_Rect* camera)
{
	//Render to screen
	for (auto text : _Text)
	{
		SDL_Rect location = text.Location;
		location.x -= camera->x;
		location.y -= camera->y; 
		SDL_RenderCopyEx(renderer, text.Texture, NULL, &location, NULL, NULL, SDL_RendererFlip::SDL_FLIP_NONE);
	}

}
bool Font::UpdateText(SDL_Renderer* renderer, std::string newValue, std::string tag)
{
	int index = GetTextIndexByTag(tag);
	if (index >= 0)
	{
		SDL_Color textColor = { 0, 255, 0 };
		SDL_Surface* textSurface = TTF_RenderText_Solid(_Font, newValue.c_str(), textColor);
		_Text[index].Texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		_Text[index].Location.w = textSurface->w;
		_Text[index].Location.h = textSurface->h;
		SDL_FreeSurface(textSurface);
		return true;
	}
	return false; 
}

bool Font::UpdateFontLocation(SDL_Rect* location, std::string tag)
{
	for (int i = 0; i < _Text.size(); i++)
	{
		if (_Text[i].Tag == tag)
		{	
			_Text[i].Location.x = location->x;
			_Text[i].Location.y = location->y;
			return true; 
		}
	}
	return false; 
}

int Font::GetTextIndexByTag(std::string tag)
{
	for (int i = 0; i < _Text.size(); i++)
	{
		if (_Text[i].Tag == tag)
			return i;
	}
	return -1;
}