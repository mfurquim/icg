#include "game.h"

int main ()
{
	Game game;

	if (game.init () < 0)
	{
		game.shut_down ();
		return -1;
	}

	game.loop ();
	game.shut_down ();

	return 0;
}