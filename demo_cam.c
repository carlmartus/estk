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
	if (esShaderLoad(&shad, "demores/cam_vert.shader", "demores/red_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (esShaderUniformRegister(&shad, 0, "un_view")) {
		printf("Cannot get uniform\n");
		return 1;
	}

	static const float lo[] = {
		0.0f, 0.0f,-1.0f,
		1.0f, 0.0f,-1.0f,
		0.0f, 1.0f,-1.0f,
	};

	esGeoBuf geobuf;
	esGeoBufCreate(&geobuf);
	esGeoBufCopy(&geobuf, lo, sizeof(lo), GEOBUF_STATIC);

	esGeo geo;
	esGeoReset(&geo, 1);
	esGeoPoint(&geo, 0, &geobuf, GEODATA_FLOAT, 3, 0, 0, ES_FALSE);

	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad);

	float mat[16];
	//esProjIdentity(mat);
	esProjPerspective(mat, 1.0f, 1.333f, 0.1f, 10.0f);
	glUniformMatrix4fv(esShaderUniformGl(&shad, 0), 1, 0, mat);

	esGeoRender(&geo, 3);

	SDL_GL_SwapBuffers();


	esGeoBufDelete(&geobuf);
	esShaderUnload(&shad);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}

