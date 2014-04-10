#ifndef ESTK_H
#define ESTK_H
#include <stdlib.h>

#define GEOBUFS_MAX 8

enum esBool {
	ES_FALSE = 0,
	ES_TRUE = 1,
};

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

void esProjIdentity(float *mat);
void esProjPerspective(
		float *mat, float fov, float screenratio, float near, float far);
void esProjLookAt(float *mat, esVec3 eye, esVec3 at, esVec3 up);
void esProjMul(float *res, float *a, float *b);

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
