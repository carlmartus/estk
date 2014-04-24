#include <stdio.h>
#include <GL/glew.h>
#include <math.h>
#include <SDL/SDL.h>
#include "estk.h"

static esSound sfx;

static void
frame(float time)
{
	static int frame_count = 0;
	if (++frame_count > 100) esGameLoopQuit();
}

static void
loop_exit()
{
	esSoundUnLoad(&sfx);
	printf("Good bye!\n");
	SDL_Quit();
}

int
main(int argc, char **argv)
{
	esGameInit(400, 300);

	if (esSoundLoad(&sfx, "demores/sound.ogg")) {
		printf("Cannot load sound file\n");
		return 1;
	}

	esSoundPlay(&sfx);

	esGameLoop(frame, loop_exit, 0);
	return 0;
}

