#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include "estk.h"

int
main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(400, 300, 0, SDL_OPENGL);

	glewInit();

	esShader shad;
	if (esShaderLoad(&shad, "demores/attr_vert.shader", "demores/attr_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	static const float attr_lo[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
	};

	static const unsigned char attr_co[] = {
		0xff, 0, 0, 0xff,
		0, 0xff, 0, 0xff,
		0, 0, 0xff, 0xff,
	};

	esGeoBuf buf_lo;
	esGeoBufCreate(&buf_lo);
	esGeoBufCopy(&buf_lo, attr_lo, sizeof(attr_lo), GEOBUF_STATIC);

	esGeoBuf buf_co;
	esGeoBufCreate(&buf_co);
	esGeoBufCopy(&buf_co, attr_co, sizeof(attr_co), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 2);
	esGeoPoint(&geo, 0, &buf_lo, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);
	esGeoPoint(&geo, 1, &buf_co, GEODATA_UBYTE, 4, 0, 0, ES_TRUE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad);
	esGeoRender(&geo, 3);

	SDL_GL_SwapBuffers();


	esGeoBufDelete(&buf_lo);
	esGeoBufDelete(&buf_co);
	esShaderUnload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

