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
	if (esShaderLoad(&shad, "demores/attr_vert.shader", "demores/attr_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	static const struct data {
		float x, y;
		unsigned char r, g, b, a;
	} data[] = {
		{ 0.0f, 0.0f, 0xff, 0, 0, 0xff },
		{ 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{ 0.0f, 1.0f, 0, 0, 0xff, 0xff },
	};

	esGeoBuf geobuf;
	esGeoBufCreate(&geobuf);
	esGeoBufCopy(&geobuf, data, sizeof(data), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 2);
	esGeoPoint(&geo, 0, &geobuf, GEODATA_FLOAT, 2, 0, sizeof(struct data), ES_FALSE);
	esGeoPoint(&geo, 1, &geobuf, GEODATA_UBYTE, 4, 8, sizeof(struct data), ES_TRUE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad);
	esGeoRender(&geo, 3);

	SDL_GL_SwapBuffers();


	esGeoBufDelete(&geobuf);
	esShaderUnload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

