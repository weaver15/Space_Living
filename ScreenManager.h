#pragma once
#include "Texture.h"
#include "Enums.cpp"
#include "AnimatedObject.h"
#include "Globals.cpp"
#include "MenuSelector.h"
#include "pugixml\pugixml.hpp"

enum ScreenType
{
	Start,
	Control,
	Options,
	Credits,
	GameOver
};

enum StartScreenOptions
{
	StartScreen_Start,
	StartScreen_Options,
	StartScreen_Exit
};

struct Option
{
	StartScreenOptions Type;
	SDL_Point Location;
	SDL_Point Cursor;
};

class ScreenManager : public AnimatedObject
{
public:
	ScreenManager();
	ScreenManager(SDL_Rect textureInfo, SDL_Rect location);
	~ScreenManager();


	void Update(SDL_Renderer* renderer, GameState* gameState);
	void Load(SDL_Renderer* renderer, SDL_Window* gWindow);
	void LoadStartScreen(SDL_Renderer* renderer);
	void LoadControlScreen(SDL_Renderer* renderer);
	void LoadCreditScreen(SDL_Renderer* renderer);
	void LoadGameOverScreen(SDL_Renderer* renderer);

	void Draw(SDL_Renderer* renderer);
	void Input(SDL_Event* e);

	bool LoadingFinished = false;

private:
	SDL_Texture* ScreenTexture; 
	Texture CursorTexture; 
	bool StartLevel = false; 
	bool Exit = false; 

	bool ShowControls = false;
	bool ShowCredits = false;
	bool ShowStart = false;
	
	SDL_Rect Camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Option Options[3];
	MenuSelector ScreenCursor;
	SDL_Surface* LoadSurface(std::string path, SDL_Surface* screenSurface);
	void LoadInfo();
	ScreenType CurrentScreen;
	SDL_Surface* ScreenSurface;
};

