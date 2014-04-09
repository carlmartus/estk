#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/gl.h>

int
main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(400, 300, 0, SDL_OPENGL);

	glClearColor(0.3, 0.4, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapBuffers();

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

