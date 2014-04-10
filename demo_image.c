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

	esTexture tex;
	if (esTextureLoad(&tex, "demores/img.png", TEX_LINEAR, TEX_NONE)) {
		printf("Cannot load image!\n");
		return 1;
	}

	esShader shad;
	if (esShaderLoad(&shad, "demores/img_vert.shader", "demores/img_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (esShaderUniformRegister(&shad, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShaderUse(&shad);
	glUniform1i(esShaderUniformGl(&shad, 0), 0);

	static const struct data {
		float x, y;
		float u, v;
		unsigned char r, g, b, a;
	} data[] = {
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0xff, 0, 0, 0xff },
		{ 1.0f, 0.0f, 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{ 0.0f, 1.0f, 0.0f, 1.0f, 0, 0, 0xff, 0xff },
	};

	esGeoBuf geobuf;
	esGeoBufCreate(&geobuf);
	esGeoBufCopy(&geobuf, data, sizeof(data), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 3);
	esGeoPoint(&geo, 0, &geobuf, GEODATA_FLOAT, 2,  0, sizeof(struct data), ES_FALSE);
	esGeoPoint(&geo, 1, &geobuf, GEODATA_FLOAT, 2,  8, sizeof(struct data), ES_TRUE);
	esGeoPoint(&geo, 2, &geobuf, GEODATA_UBYTE, 4, 16, sizeof(struct data), ES_TRUE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	esShaderUse(&shad);
	esTextureUse(&tex);
	esGeoRender(&geo, 3);

	SDL_GL_SwapBuffers();

	esTextureUnload(&tex);
	esGeoBufDelete(&geobuf);
	esShaderUnload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

