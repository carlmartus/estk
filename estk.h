#ifndef ESTK_H
#define ESTK_H
#include <stdlib.h>

enum esBool {
	ES_FALSE = 0,
	ES_TRUE = 1,
};

// Game loop
void esGameInit(int screen_width, int screen_height);
void esGameGlSwap();
void esGameLoop(void (*frame)(float t), void (*exit)(), int frame_rate);
void esGameLoopQuit();
void esGameRegisterKey(int sdlkey, void (*callback)(int key, int down));

// Shader
typedef int esUniform;
#define UNIFORMS_MAX 10

typedef struct {
	int glprogram;
	esUniform uniforms[UNIFORMS_MAX];
} esShader;

int esShaderLoad(esShader *shader, const char *vert_file, const char *frag_file);
void esShaderUse(const esShader *shader);
void esShaderUnload(esShader *shader);
int esShaderUniformRegister(esShader *shader, esUniform reg, const char *name);
int esShaderUniformGl(esShader *shader, esUniform reg);

// Geometry buffer
#define GEOBUFS_MAX 8

enum esGeoBufType {
	GEOBUF_STATIC,
	GEOBUF_DYNAMIC,
	GEOBUF_STREAM,
};

typedef struct {
	unsigned int glbuf;
} esGeoBuf;

void esGeoBufCreate(esGeoBuf *buf);
void esGeoBufCopy(esGeoBuf *buf,
		const void *data, size_t size, enum esGeoBufType type);
void esGeoBufDelete(esGeoBuf *buf);

// Geometry
enum esGeoDataType {
	GEODATA_FLOAT,
	GEODATA_INT,
	GEODATA_BYTE,
	GEODATA_UBYTE,
};

typedef struct {
	int bufcount;

	struct {
		esGeoBuf *geobuf;
		enum esGeoDataType datatype;
		int elements;
		size_t offset, stride;
		enum esBool normalized;
	} buf[GEOBUFS_MAX];
} esGeo;

void esGeoReset(esGeo *geo, int bufcount);
void esGeoPoint(esGeo *geo, int id, esGeoBuf *geobuf, enum esGeoDataType datatype,
		int elements, size_t offset, size_t stride, enum esBool normalized);
void esGeoRender(const esGeo *geo, int vertices);

// Projection
typedef struct { float x, y, z; } esVec3;

void esProjOrtho(float *mat, float x0, float y0, float x1, float y1);
void esProjPerspective(
		float *mat, float fov, float screenratio, float near, float far,
		esVec3 eye, esVec3 at, esVec3 up);

// Texture
enum esTextureMipmap {
	TEX_NONE,
	TEX_LINEAR,
};

typedef struct {
	int w, h;
	int gltexture;
} esTexture;

int esTextureLoad(esTexture *tex, const char *file_name,
		enum esTextureMipmap min, enum esTextureMipmap mag);
void esTextureUse(esTexture *tex);
void esTextureUnload(esTexture *tex);

#endif

