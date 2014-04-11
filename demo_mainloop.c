#include <stdio.h>
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <math.h>
#include "estk.h"

static esShader shad;
static esGeo geo;
static esGeoBuf geobuf;

static void
frame(float time)
{
	static int frame_count = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad);

	//float mat[16], persp[16], look[16];
	//esProjPerspective(persp, 1.3f, 1.333f, 0.1f, 20.0f);

	float v = 0.01f * (float) frame_count;
	//esVec3 cam_ey = {-1.0f, v, 1.0f };
	esVec3 cam_ey = { 0.0f + 1.0f*cosf(v*4.0f), 1.0f*sinf(v*4.0f), 1.0f };
	esVec3 cam_at = { 0.0f, 0.0f, 0.0f };
	esVec3 cam_up = { 0.0f, 0.0f, 1.0f };

	/*
	esProjLookAt(look, cam_ey, cam_at, cam_up);
	esProjMul(mat, persp, look);*/

	float mat[16];
	esProjPerspective(mat, 1.5f-v, 1.333f, 0.1f, 20.0f, cam_ey, cam_at, cam_up);

	glUniformMatrix4fv(esShaderUniformGl(&shad, 0), 1, 0, mat);

	esGeoRender(&geo, 3);

	SDL_GL_SwapBuffers();

	if (++frame_count > 100) esGameLoopQuit();
}

int
main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode(400, 300, 0, SDL_OPENGL);

	glewInit();

	if (esShaderLoad(&shad, "demores/cam_vert.shader", "demores/red_frag.shader")) {
		printf("Cannot load shaders!\n");
		return 1;
	}

	if (esShaderUniformRegister(&shad, 0, "un_view")) {
		printf("Cannot get uniform\n");
		return 1;
	}

	static const float lo[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	esGeoBufCreate(&geobuf);
	esGeoBufCopy(&geobuf, lo, sizeof(lo), GEOBUF_STATIC);

	esGeoReset(&geo, 1);
	esGeoPoint(&geo, 0, &geobuf, GEODATA_FLOAT, 3, 0, 0, ES_FALSE);

	glClearColor(0.6, 0.5, 0.6, 1.0);

	esGameLoop(frame, 0);

	esGeoBufDelete(&geobuf);
	esShaderUnload(&shad);

	SDL_Quit();
	return 0;
}

