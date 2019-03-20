#pragma once
#include <SDL.h>
class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void Stop();
	void Reset();
	int GetTicks();
	bool IsStarted();

private:
	//1000 = 1 sec
	int Ticks= 0;
	bool Started = false;
};

