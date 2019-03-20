#pragma once
#include <SDL.h>
#include <string>
#include "OpenGL.h"
#include "ErrorManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <cmath>
#include <vector>

struct Text
{
public:
	std::string Value;
	SDL_Rect Location; 
	SDL_Texture* Texture;
	std::string Tag; 
};
class Font
{
public:
	Font();
	~Font();
	void Free();
	void LoadFont(SDL_Renderer* renderer, int fontSize = 22);
	bool AddText(SDL_Renderer* renderer, std::string textureText, SDL_Rect* location, std::string tag, SDL_Color textColor);
	bool UpdateText(SDL_Renderer* renderer, std::string newValue, std::string tag);
	bool UpdateFontLocation(SDL_Rect* location, std::string tag);
	
	//HUD text and text that won't move on screen
	void RenderText(SDL_Renderer* renderer);

	//Render Text in respect to the camera
	void RenderText(SDL_Renderer* renderer, SDL_Rect* camera);



private:
	std::vector<Text> _Text;
	TTF_Font * _Font = NULL;
	int GetTextIndexByTag(std::string tag);
};

