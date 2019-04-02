#pragma once
#include <SDL.h>
#include <stdio.h>

enum ControllerType
{
	Keyboard,
	Gamepad
};

class Controller
{
public:

	Controller();
	~Controller();

	ControllerType GetControllerType();
	void SetControllerType(ControllerType type);
	void Input(SDL_Event e);

	//Analog joystick dead zone
	const int JOYSTICK_DEAD_ZONE = 8000;
	//Game Controller 1 handler
	SDL_Joystick* gGameController = NULL;

private:
	ControllerType CurrentControllerType;

	void GamePadInput(SDL_Event e);
	void KeyboardInput(SDL_Event e);

};

