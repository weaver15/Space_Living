#pragma once
#include "Timer.h"
Timer::Timer()
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
	Started = true;
	Ticks = SDL_GetTicks();
}
void Timer::Stop()
{
	Started = false;
}


int Timer::GetTicks()
{
	if (Started == true)
	{
		return SDL_GetTicks() - Ticks;
	}
	return 0;
}

bool Timer::IsStarted()
{
	return Started;
}

void Timer::Reset()
{
	Ticks = 0;
	Started = false;
}


