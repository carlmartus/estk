#include <stdio.h>
#include <SDL/SDL.h>
#include "estk.h"

static void
loop_frame(float fr)
{
}

static void
callback_quit(int sdlkey, int down)
{
	esGameLoopQuit();
}

static void
callback_message(int sdlkey, int down)
{
	printf("Message! down=%d\n", down);
}

int
main(int argc, char **argv)
{
	esGameInit(400, 300);

	esGameRegisterKey(SDLK_q, callback_quit);
	printf("Press 'q' to exit\n");

	esGameRegisterKey(SDLK_m, callback_message);
	printf("Press 'm' to print message\n");

	esGameLoop(loop_frame, NULL, 0);
	return 0;
}

