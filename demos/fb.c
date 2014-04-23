#include <stdio.h>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include "estk.h"

int
main(int argc, char **argv)
{
	esGameInit(400, 300);

	esShader shad_red;
	if (esShaderLoad(&shad_red,
				"demores/red_vert.shader", "demores/red_frag.shader")) {
		printf("Cannot load shad_reders!\n");
		return 1;
	}

	static const float cube_lo[] = {
		-0.4f, -0.4f,
		 0.4f, -0.3f,
		-0.5f,  0.5f,
		 0.4f, -0.3f,
		 0.5f,  0.5f,
		-0.5f,  0.5f,
	};

	esShader shad_tex;
	if (esShaderLoad(&shad_tex,
				"demores/img_vert.shader", "demores/img_frag.shader")) {
		printf("Cannot load shad_reders!\n");
		return 1;
	}

	if (esShaderUniformRegister(&shad_tex, 0, "un_tex0")) {
		printf("Cannot get uniform constant\n");
		return 1;
	}

	esShaderUse(&shad_tex);
	glUniform1i(esShaderUniformGl(&shad_tex, 0), 0);

	esFrameBuffer fb;
	if (esFrameBufferCreate(&fb, 64)) {
		printf("Cannot create frame buffer\n");
		return 1;
	}

	// Red cube
	esGeoBuf cubebuf;
	esGeoBufCreate(&cubebuf);
	esGeoBufCopy(&cubebuf, cube_lo, sizeof(cube_lo), GEOBUF_STATIC);

	esGeo cube;
	esGeoReset(&cube, 1);
	esGeoPoint(&cube, 0, &cubebuf, GEODATA_FLOAT, 2, 0, 0, ES_FALSE);

	// Textured cube
	static const struct data {
		float x, y;
		float u, v;
		unsigned char r, g, b, a;
	} data[] = {
		{ -1.0f, -1.0f, 0.0f, 0.0f, 0xff, 0, 0, 0xff },
		{  1.0f, -1.0f, 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{ -1.0f,  1.0f, 0.0f, 1.0f, 0, 0, 0xff, 0xff },

		{  1.0f, -1.0f, 1.0f, 0.0f, 0, 0xff, 0, 0xff },
		{  1.0f,  1.0f, 0.0f, 0.0f, 0xff, 0xff, 0xff, 0xff },
		{ -1.0f,  1.0f, 0.0f, 1.0f, 0, 0, 0xff, 0xff },
	};

	esGeoBuf geobuf;
	esGeoBufCreate(&geobuf);
	esGeoBufCopy(&geobuf, data, sizeof(data), GEOBUF_STATIC);

	esGeo texgeo;
	esGeoReset(&texgeo, 3);
	esGeoPoint(&texgeo, 0, &geobuf, GEODATA_FLOAT, 2,  0,
			sizeof(struct data), ES_FALSE);
	esGeoPoint(&texgeo, 1, &geobuf, GEODATA_FLOAT, 2,  8,
			sizeof(struct data), ES_TRUE);
	esGeoPoint(&texgeo, 2, &geobuf, GEODATA_UBYTE, 4, 16,
			sizeof(struct data), ES_TRUE);

	// Render to texture
	esFrameBufferSet(&fb);
	glClearColor(0.6, 0.5, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	esShaderUse(&shad_red);
	esGeoRender(&cube, 6);
	esFrameBufferUnSet();

	// Render geometry
	esFrameBufferBind(&fb);
	esShaderUse(&shad_tex);
	esGeoRender(&texgeo, 6);

	esGameGlSwap();
	esFrameBufferDelete(&fb);
	esGeoBufDelete(&cubebuf);
	esShaderUnload(&shad_red);

	SDL_Delay(800);
	SDL_Quit();
	return 0;
}


