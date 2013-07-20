#include "timer.h"
#include <SDL/SDL.h>

Timer::Timer ()
{
	start_ticks = 0;
	paused_ticks = 0;
	paused = false;
	started = false;
}

void
Timer::wait_diff (float ratio_fps)
{
	SDL_Delay (ratio_fps - get_ticks ());

	return ;
}


void
Timer::start ()
{
	started = true;
	paused = false;
	start_ticks = SDL_GetTicks ();

	return ;
}

void
Timer::stop ()
{
	started = false;
	paused = false;

	return ;
}

void
Timer::pause ()
{
	if ((started == true) && (paused == false))
	{
		paused = true;
		paused_ticks = SDL_GetTicks () - start_ticks;
	}

	return ;
}

void
Timer::unpause ()
{
	if (paused == true)
	{
		paused = false;
		start_ticks = SDL_GetTicks () - paused_ticks;
		paused_ticks = 0;
	}

	return ;
}

int
Timer::get_ticks ()
{
	if (started == true)
	{
		if (paused == true)
		{
			return paused_ticks;
		}
		else
		{
			return SDL_GetTicks () - start_ticks;
		}
	}

	return 0;
}

bool
Timer::is_started ()
{
	return started;
}

bool
Timer::is_paused ()
{
	return paused;
}
