#include "ScreenManager.h"



ScreenManager::ScreenManager()
{
}

ScreenManager::ScreenManager(SDL_Rect textureInfo, SDL_Rect location) : AnimatedObject(textureInfo, location)
{
	SetIsAlive(true);
}

ScreenManager::~ScreenManager()
{
	SetIsAlive(false);
}

void ScreenManager::Update(SDL_Renderer* renderer, GameState* gameState)
{
	if (StartLevel)
	{
		StartLevel = false; 
		*(gameState) = GameState::Active_Level;
	}
	else if (Exit)
	{
		Exit = false; 
		*(gameState) = GameState::Exit;
	}
	else if (*(gameState) == GameState::GameOverScreen)
	{
		LoadGameOverScreen(renderer);
		*(gameState) = GameState::Screen;
	}
	else if (*(gameState) == GameState::CreditsScreen)
	{
		LoadCreditScreen(renderer);
		*(gameState) = GameState::Screen;
	}
	else if (ShowControls)
	{
		LoadControlScreen(renderer);
	}
	else if (ShowStart)
	{
		LoadStartScreen(renderer);
	}
	else
	{
		Draw(renderer);
	}
}

void ScreenManager::Load(SDL_Renderer* renderer, SDL_Window* gWindow)
{
	ScreenSurface = SDL_GetWindowSurface(gWindow);
	LoadStartScreen(renderer);
	CursorTexture.InitTexture(renderer, gWindow, "StartScreen");

	LoadingFinished = true;
}

void ScreenManager::LoadStartScreen(SDL_Renderer* renderer)
{
	SDL_Surface* stretchedSurface = LoadSurface("Texture/StartScreen.bmp", ScreenSurface);
	CurrentScreen = ScreenType::Start;
	if (stretchedSurface == NULL)
		printf("Failed to load stretching image!\n");
	ScreenTexture = SDL_CreateTextureFromSurface(renderer, stretchedSurface);
	ScreenCursor.SetIsAlive(true);
	LoadInfo();
	ShowStart = false; 
}

void ScreenManager::LoadControlScreen(SDL_Renderer* renderer)
{
	SDL_Surface* stretchedSurface = LoadSurface("Texture/ControlScreen.bmp", ScreenSurface);
	CurrentScreen = ScreenType::Control;
	if (stretchedSurface == NULL)
		printf("Failed to load stretching image!\n");
	ScreenTexture = SDL_CreateTextureFromSurface(renderer, stretchedSurface);
	ScreenCursor.SetIsAlive(false);
	ShowControls = false; 
}

void ScreenManager::LoadCreditScreen(SDL_Renderer* renderer)
{
	SDL_Surface* stretchedSurface = LoadSurface("Texture/CreditsScreen.bmp", ScreenSurface);
	CurrentScreen = ScreenType::Credits;
	if (stretchedSurface == NULL)
		printf("Failed to load stretching image!\n");
	ScreenTexture = SDL_CreateTextureFromSurface(renderer, stretchedSurface);
	ScreenCursor.SetIsAlive(false);
	ShowCredits = false;
}

void ScreenManager::LoadGameOverScreen(SDL_Renderer* renderer)
{
	SDL_Surface* stretchedSurface = LoadSurface("Texture/GameOverScreen.bmp", ScreenSurface);
	CurrentScreen = ScreenType::GameOver;
	if (stretchedSurface == NULL)
		printf("Failed to load stretching image!\n");
	ScreenTexture = SDL_CreateTextureFromSurface(renderer, stretchedSurface);
	ScreenCursor.SetIsAlive(false);
}

SDL_Surface* ScreenManager::LoadSurface(std::string path, SDL_Surface* screenSurface)
{
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return optimizedSurface;
}

void ScreenManager::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, ScreenTexture, &TextureInformation, &Camera);
	ScreenCursor.Draw(renderer, &CursorTexture, &Camera);
}

void ScreenManager::Input(SDL_Event* e)
{
	SDL_Keycode keyPressed = e->key.keysym.sym;
	if (CurrentScreen == ScreenType::Start)
	{
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
		{
			if (keyPressed == SDLK_w)
				ScreenCursor.MoveSelector(Direction::Up);
			if (keyPressed == SDLK_s)
				ScreenCursor.MoveSelector(Direction::Down);
			if (keyPressed == SDLK_RETURN)
			{
				if (Options[ScreenCursor.GetRow()].Type == StartScreenOptions::StartScreen_Start)
					ShowControls = true;
				if (Options[ScreenCursor.GetRow()].Type == StartScreenOptions::StartScreen_Exit)
					Exit = true;
			}
		}
	}
	else if (CurrentScreen == ScreenType::Control)
	{
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0 && keyPressed == SDLK_RETURN)
			StartLevel = true;
	}
	else if (CurrentScreen == ScreenType::Credits)
	{
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0 && keyPressed == SDLK_RETURN)
			ShowStart = true;
	}
	else if (CurrentScreen == ScreenType::GameOver)
	{
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0 && keyPressed == SDLK_RETURN)
			ShowStart = true;
	}

}

void ScreenManager::LoadInfo()
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("GameInfo.xml");
	pugi::xml_node screen = doc.child("StartScreen");

	SDL_Point start = {
		screen.child("Start").attribute("x").as_int(),
		screen.child("Start").attribute("y").as_int()
	};
	SDL_Point start_cursor = {
		screen.child("Start").attribute("x1").as_int(),
		screen.child("Start").attribute("y1").as_int()
	};
	Options[0].Type = StartScreenOptions::StartScreen_Start;
	Options[0].Location = start;
	Options[0].Cursor = start_cursor;

	SDL_Point exit = {
		screen.child("Exit").attribute("x").as_int(),
		screen.child("Exit").attribute("y").as_int()
	};
	SDL_Point exit_cursor = {
		screen.child("Exit").attribute("x1").as_int(),
		screen.child("Exit").attribute("y1").as_int()
	};
	Options[1].Type = StartScreenOptions::StartScreen_Exit;
	Options[1].Location = exit;
	Options[1].Cursor = exit_cursor; 

	SDL_Rect cursor = {
		screen.child("Cursor").attribute("x").as_int(),
		screen.child("Cursor").attribute("y").as_int(),
		screen.child("Cursor").attribute("width").as_int(),
		screen.child("Cursor").attribute("height").as_int()
	};

	double cursorXLocation = SCREEN_WIDTH / 2 - cursor.w / 2;
	SDL_Point startScreenDim = { screen.attribute("width").as_int(), screen.attribute("height").as_int() };

	double cursorYLocation = (SCREEN_HEIGHT / (double)startScreenDim.y) * Options[0].Cursor.y;
	SDL_Rect location = { cursorXLocation, cursorYLocation, cursor.w, cursor.h };
	int startScreenCount = screen.attribute("count").as_int();
	ScreenCursor = MenuSelector(cursor, location, { 1,1 }, {0, startScreenCount });
	
	double cursorYLocation2 = (SCREEN_HEIGHT / (double)startScreenDim.y) * Options[1].Cursor.y;
	ScreenCursor.SetDrawOffset(cursorYLocation2 - cursorYLocation);
}