#include "Controller.h"



Controller::Controller()
{
	if (SDL_NumJoysticks() >= 1)
	{
		gGameController = SDL_JoystickOpen(0);
		if (gGameController == NULL)
		{
			printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}
}


Controller::~Controller()
{
	SDL_JoystickClose(gGameController);
	gGameController = NULL;
}

ControllerType Controller::GetControllerType()
{
	return CurrentControllerType;
}

void Controller::SetControllerType(ControllerType type)
{
	CurrentControllerType = type; 
}

void Controller::Input(SDL_Event e)
{
	//Normalized direction
	if (CurrentControllerType == Gamepad)
	{
		GamePadInput(e);
	}
	else if (CurrentControllerType == Keyboard)
	{
		KeyboardInput(e);
	}
}

void Controller::GamePadInput(SDL_Event e)
{
	int xDir = 0;
	int yDir = 0;
	if (e.type == SDL_JOYAXISMOTION)
	{
		//Motion on controller 0
		if (e.jaxis.which == 0)
		{
			//X axis motion
			if (e.jaxis.axis == 0)
			{
				//Left of dead zone
				if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				{
					xDir = -1;
				}
				//Right of dead zone
				else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				{
					xDir = 1;
				}
				else
				{
					xDir = 0;
				}
			}
			//Y axis motion
			else if (e.jaxis.axis == 1)
			{
				//Below of dead zone
				if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
				{
					yDir = -1;
				}
				//Above of dead zone
				else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
				{
					yDir = 1;
				}
				else
				{
					yDir = 0;
				}
			}
		}
		double joystickAngle = atan2((double)yDir, (double)xDir) * (180.0 / M_PI);
		//Correct angle
		if (xDir == 0 && yDir == 0)
		{
			joystickAngle = 0;
		}
	}
}

void Controller::KeyboardInput(SDL_Event e)
{

}

