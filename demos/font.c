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
				"demores/font_vert.shader",
				"demores/font_frag.shader")) {
		printf("Cannot open shader\n");
		return 1;
	}

	if (esShaderUniformRegister(&shad, 0, "un_mvp")) {
		printf("Cannot register mvp uniform\n");
		return 1;
	} else {
		esShaderUse(&shad);
		float mat[16];
		esProjOrtho(mat, -10.0f, 10.0f, 10.0f, -10.0f);
		glUniformMatrix4fv(esShaderUniformGl(&shad, 0), 1, 0, mat);
	}

	if (esShaderUniformRegister(&shad, 1, "un_tex0")) {
		printf("Cannot register texture uniform\n");
		return 1;
	} else {
		glUniform1i(esShaderUniformGl(&shad, 1), 0);
	}

	esFont font;
	esFontCreate(&font, &bitmap, &shad, 0, 1, 0);
	esFontAddText(&font, -4.0f, -2.0f, "Hejsan!");

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esFontRender(&font);
	esFontClearBuf(&font);

	esGameGlSwap();

	esFontDelete(&font);
	esShaderUnload(&shad);
	esTextureUnload(&bitmap);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

