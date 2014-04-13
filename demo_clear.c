#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "estk.h"

int
main(int argc, char **argv)
{
	esGameInit(400, 300);

	glClearColor(0.3, 0.4, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esGameGlSwap();

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

