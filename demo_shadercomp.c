#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int
main()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(400, 300, 0, SDL_OPENGL);

	glewInit();

	esShader shad;
	if (esShaderLoad(&shad, "demores/red_vert.shader", "demores/red_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}
	esShaderUnload(&shad);

	SDL_Quit();
	return 0;
}

