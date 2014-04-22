#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int
main(int argc, char **argv)
{
	esGameInit(400, 300);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esTexture bitmap;
	if (esTextureLoad(&bitmap,
				"demores/font.png", TEX_LINEAR, TEX_LINEAR)) {
		printf("Cannot open bitmap font\n");
		return 1;
	}

	esShader shad;
	if (esShaderLoad(&shad,
				"demores/img_vert.shader",
				"demores/img_frag.shader")) {
		printf("Cannot open shader\n");
		return 1;
	}

	esFont font;
	esFontCreate(&font, &bitmap, &shad, 0, 1, 2);
	esFontAddText(&font, 0.0f, 0.0f, "Hello");
	esFontRender(&font);

	esGameGlSwap();

	esFontDelete(&font);
	esShaderUnload(&shad);
	esTextureUnload(&bitmap);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

