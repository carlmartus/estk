#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int
main(int argc, char **argv)
{
	esGameInit(400, 300);

	esShader shad;
	if (esShaderLoad(&shad, "demores/red_vert.shader", "demores/red_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}
	esShaderUnload(&shad);

	SDL_Quit();
	return 0;
}

