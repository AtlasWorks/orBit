// ----------------------------------------------------
// j1Timer.cpp
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

// ---------------------------------------------
j1Timer::j1Timer()
{
	Start();
}

// ---------------------------------------------
void j1Timer::Start()
{
	started_at = SDL_GetTicks();
}

// ---------------------------------------------
uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at - loadX;
}

// ---------------------------------------------
float j1Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at-loadX) / 1000.0f;
}

// ---------------------------------------------
uint32 j1Timer::startreturn() 
{
	return started_at;
}

// ---------------------------------------------
void j1Timer::LoadXtime(uint32 loaded)
{
	uint lol = SDL_GetTicks() - started_at;
	loadX = lol - loaded;
}

void j1Timer::ResetX()
{
	loadX = 0;
}