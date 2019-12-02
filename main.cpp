#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include "Level.h"
#include "ScreenManager.h"
#include <ctime>


SDL_Window* Window = NULL;
SDL_Renderer* Renderer = NULL;
Level _Level;
ScreenManager _ScreenManager;
bool Quit = false;
Timer FPS_Timer;

//GameState CurrentGameState = GameState::Screen;
GameState CurrentGameState = GameState::Active_Level;

bool playBGMusic = false;
Mix_Music* gMusic = NULL;
Mix_Chunk *gShot = NULL;

int Close()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	Window = NULL;
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 0;
}

bool Init2()
{
	bool success = true;

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		success = false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!\n");
		}

		//Create window
		Window = SDL_CreateWindow("Space Living", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (Window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (Renderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}
	if(success)
		printf("Main initialized\n");
	return success;
}

void Init()
{
	//Loading success flag
	bool success = true;
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	Window = SDL_CreateWindow("Space Living", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	IMG_Init(IMG_INIT_PNG);



	//Load music
	gMusic = Mix_LoadMUS("Audio/Anxiety.mp3");
	if (gMusic == NULL)
	{
		printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	else if(playBGMusic == true)
	{
		Mix_PlayMusic(gMusic, -1);
	}
	 
	//Load sound effects
	gShot = Mix_LoadWAV("Audio/laser2.ogg");
	if (gShot == NULL)
	{
		printf("Failed to load laser1 sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = false;
	}
	//Mix_PlayChannel(-1, gShot, 0);

}

void LoadLevel()
{
	if (!_Level.IsLoaded())
	{
		_Level.Load(Renderer);
		_Level.LoadMedia(Renderer, Window);
	}
}

void LoadStartScreen()
{
	if (!_ScreenManager.LoadingFinished)
	{
		SDL_Rect texture = { 0, 0, 800, 600 };
		SDL_Rect location = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		_ScreenManager = ScreenManager(texture, location);
		_ScreenManager.Load(Renderer, Window);
	}
}

void LoadMedia()
{

	switch (CurrentGameState)
	{
		case Screen:
			LoadStartScreen();
			break;
		case Active_Level:
			LoadLevel();
			break;
		default:
			break;
	}
}

void Input(SDL_Event e)
{
	if (e.type == SDL_QUIT)
	{
		Quit = true;
	}
	else
	{
		switch (CurrentGameState)
		{
		case Screen:
		case GameOverScreen:
		case CreditsScreen:
			_ScreenManager.Input(&e);
			break;
		case Active_Level:
			_Level.PlayerInput(&e, Renderer);
			break;
		case Exit:
			Quit = true;
			break;
		default:

			break;
		}
	}
}

void Update()
{

	switch (CurrentGameState)
	{
	case Screen:
	case GameOverScreen:
	case CreditsScreen:
		_ScreenManager.Update(Renderer, &CurrentGameState);
		break;
	case Active_Level:
		_Level.Update(Renderer, &CurrentGameState);
		break;
	default:
		break;
	}
}

void FPSDelay()
{
	int frameTicks = FPS_Timer.GetTicks();
	if (frameTicks < SCREEN_TICKS_PER_FRAME)
	{
		SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
	}
}

int main(int argc, char* args[])
{
	Init();
	LoadMedia();


	srand(time(0));
	SDL_Event e;

	
	while (!Quit)
	{
		FPS_Timer.Start();

		//Player input
		while (SDL_PollEvent(&e) != 0)
		{
			Input(e);
		}

		//This is called to check if the screen has changed and another screen should be loaded
		LoadMedia();

		SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(Renderer);

		//Screen updates
		Update();

		SDL_RenderPresent(Renderer);

		//Stay within max fps
		FPSDelay();
	}
	return Close();
}
