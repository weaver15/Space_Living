#pragma once
#include <SDL.h>
#include <string>
#include "OpenGL.h"
#include "ErrorManager.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <cmath>

class Texture
{
public:
	Texture();
	~Texture();
	void free();
	
	bool LoadTexture(std::string path, SDL_Renderer* renderer, SDL_Window* gWindow);
	bool LoadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor);
	bool LoadFont(SDL_Renderer* renderer);
	void InitTexture(SDL_Renderer* renderer, SDL_Window* gWindow, std::string fileName);
	void Render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, std::string tag = NULL);
	//void SetTextureName(std::string name);
	void SetTexture(SDL_Texture* texture);
	void SetSize(int width, int height);


private:

	SDL_Texture* _Texture;
	void* Pixels;
	int Pitch;
	SDL_Point* Size;
	SDL_Point* ModifiedSize;
	std::string TextureName; 
	TTF_Font *Font = NULL;
};