#pragma once
#include "Texture.h"


Texture::Texture()
{
	_Texture = NULL;
	Size = NULL;
	Pixels = NULL;
	ModifiedSize = NULL;
	Pitch = 0;
}

Texture::~Texture()
{
	if (_Texture != NULL)
	{
		SDL_DestroyTexture(_Texture);
		_Texture = NULL;
		Size = NULL;
		Pixels = NULL;
		Pitch = 0;
		ModifiedSize = NULL;
	}
}
void Texture::free()
{
	if (_Texture != NULL)
	{
		SDL_DestroyTexture(_Texture);
		_Texture = NULL;
		Size = NULL;
		TTF_CloseFont(Font);
		Font = NULL;
		ModifiedSize = NULL;
	}
}
bool Texture::LoadTexture(std::string path, SDL_Renderer* renderer, SDL_Window* gWindow)
{

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	SDL_Surface* formattedSurface = SDL_ConvertSurface(loadedSurface, SDL_GetWindowSurface(gWindow)->format, NULL);
	SDL_Texture* newTexture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(gWindow), SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);

	SDL_LockTexture(newTexture, NULL, &Pixels, &Pitch);
	memcpy(Pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);
	SDL_UnlockTexture(newTexture);
	
	Pixels = NULL;
	*Size = SDL_Point{ formattedSurface->w, formattedSurface->h };
	TextureName = path;
	_Texture = newTexture;

	SDL_FreeSurface(formattedSurface);
	SDL_FreeSurface(loadedSurface);
	return _Texture != NULL;
}

void Texture::SetTexture(SDL_Texture* texture)
{
	_Texture = texture; 
}


void Texture::Render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, std::string tag)
{
	SDL_Rect renderQuad = { x, y,0, 0};

	if (Size != NULL)
	{
		renderQuad.w = Size->x;
		renderQuad.h = Size->y;
	}

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	if (ModifiedSize != NULL)
	{
		renderQuad.h = ModifiedSize->y;
		renderQuad.w = ModifiedSize->x;
	}

	SDL_RenderCopyEx(renderer, _Texture, clip, &renderQuad, angle, center, flip);
}

void Texture::SetSize(int width, int height)
{
	*&ModifiedSize = new SDL_Point{ width , height };
}

bool Texture::LoadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor)
{
	free();
	SDL_Surface* textSurface = TTF_RenderText_Solid(Font, textureText.c_str(), textColor);
	_Texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	*Size = SDL_Point{ textSurface->w, textSurface->h };
	SDL_FreeSurface(textSurface);

	return _Texture != NULL;
}

bool Texture::LoadFont(SDL_Renderer* renderer)
{
	bool success = true;
	//Open the font
	Font = TTF_OpenFont("Texture/font.TTF", 28);
	//Render text
	SDL_Color textColor = { 0, 0, 0 };
	return success;
}


void Texture::InitTexture(SDL_Renderer* renderer, SDL_Window* gWindow, std::string fileName)
{
	std::string fileLocation = "Texture/" + fileName + ".bmp";
	SDL_Surface* loadedSurface = SDL_LoadBMP(fileLocation.c_str());
	SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(SDL_GetWindowSurface(gWindow)->format, 0, 0xFF, 0xFF));
	SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	SetTexture(loadedTexture);
}
