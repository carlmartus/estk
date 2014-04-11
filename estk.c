// Top {{{
#include "estk.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>
#include <GL/glew.h>
#ifdef EMSCRIPTEN
#include <emscripten/emscripten.h>
#endif

#include <SDL/SDL_image.h>

static void
_check_error(int line)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		printf("OpenGL Error (:%d): %d ", line, err);

		switch (err) {
#define ERR_CASE_PRINT(cond) case cond : printf("%s\n", #cond); break
			ERR_CASE_PRINT(GL_INVALID_ENUM);
			ERR_CASE_PRINT(GL_INVALID_VALUE);
			ERR_CASE_PRINT(GL_INVALID_OPERATION);
			ERR_CASE_PRINT(GL_STACK_OVERFLOW);
			ERR_CASE_PRINT(GL_STACK_UNDERFLOW);
			ERR_CASE_PRINT(GL_OUT_OF_MEMORY);
			default : break;
		}
	}
}
#define check_error() _check_error(__LINE__)

// }}}
// Game loop {{{
static int loop_run;

#ifdef EMSCRIPTEN
static float emscripten_frame_time;
static void (*emscripten_frame)(float t);

static void
emscripten_mainloop()
{
}

#endif

void
esGameLoop(void (*frame)(float t), int frame_rate)
{
	loop_run = 1;

#ifdef EMSCRIPTEN
	emscripten_frame = frame;
	emscripten_frame_time = 1.0f / (float) frame_rate;

	emscripten_set_main_loop(emscripten_mainloop, frame_rate, 0);
#else

	if (frame_rate == 0) frame_rate = 60;
	int loop_delay = 1000 / frame_rate;

	while (loop_run) {
		frame(0.1f);
		SDL_Delay(loop_delay);
	}
#endif
}

void
esGameLoopQuit()
{
	loop_run = 0;
}

// }}}
// File {{{

static void*
file_alloc(const char *file_name)
{
	FILE *fd = fopen(file_name, "r");
	if (fd == NULL) return NULL;
	struct stat st;

	stat(file_name, &st);

	char *buf = malloc(st.st_size + 1);
	fread(buf, st.st_size, 1, fd);
	fclose(fd);

	buf[st.st_size] = '\0';

	//printf("Loaded %s (%d b)\n%s", file_name, (int) st.st_size, buf);
	return buf;
}

// }}}
// Shader {{{

static int
check_shader(GLuint id, const char *shader_info)
{
	GLint result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		char info_buf[500];
		glGetShaderInfoLog(id, sizeof(info_buf)-1, NULL, info_buf);
		printf("Shader info (%s):\n%s\n", shader_info, info_buf);
		check_error();
		return 1;
	}

	return 0;
}

int
shader_load(const char *file_name, GLenum shader_type, const char *shader_info)
{
	char *content = file_alloc(file_name);
	if (content == NULL) {
		return 0;
	}

	int shad = glCreateShader(shader_type);
	glShaderSource(shad, 1, (const char**) &content , NULL);
	glCompileShader(shad);
	check_error();
	free(content);

	if (check_shader(shad, shader_info)) return 0;

	return shad;
}

int
esShaderLoad(esShader *shader, const char *vert_file, const char *frag_file)
{
	int idvert = shader_load(vert_file, GL_VERTEX_SHADER, "Vertex shader");
	if (idvert == 0) {
		printf("Invalid vertex shader file (%s)\n", vert_file);
		return 1;
	}

	int idfrag = shader_load(frag_file, GL_FRAGMENT_SHADER, "Fragment shader");
	if (idfrag == 0) {
		printf("Invalid fragment shader file (%s)\n", frag_file);
		return 1;
	}

	int program = glCreateProgram();

	glAttachShader(program, idvert);
	check_error();
	glAttachShader(program, idfrag);
	check_error();

	glLinkProgram(program);
	check_error();

	glDeleteShader(idvert);
	glDeleteShader(idfrag);

	shader->glprogram = program;
	return 0;
}

void
esShaderUse(const esShader *shader)
{
	glUseProgram(shader->glprogram);
}

void
esShaderUnload(esShader *shader)
{
	glDeleteShader(shader->glprogram);
}

int
esShaderUniformRegister(esShader *shader, esUniform reg, const char *name)
{
	int loc = glGetUniformLocation(shader->glprogram, name);
	if (loc < 0) return 1;

	shader->uniforms[reg] = loc;
	return 0;
}

int
esShaderUniformGl(esShader *shader, esUniform reg)
{
	return shader->uniforms[reg];
}

// }}}
// Geometry buf {{{

void
esGeoBufCreate(esGeoBuf *buf)
{
	glGenBuffers(1, &buf->glbuf);
	check_error();
}

void
esGeoBufCopy(esGeoBuf *buf, const void *data, size_t size, enum esGeoBufType type)
{
	static const unsigned int map[] = {
		[GEOBUF_STATIC] = GL_STATIC_DRAW,
		[GEOBUF_DYNAMIC] = GL_DYNAMIC_DRAW,
		[GEOBUF_STREAM] = GL_STREAM_DRAW,
	};

	glBindBuffer(GL_ARRAY_BUFFER, buf->glbuf);
	check_error();
	glBufferData(GL_ARRAY_BUFFER, size, data, map[type]);
	check_error();
}

void
esGeoBufDelete(esGeoBuf *buf)
{
	glDeleteBuffers(1, &buf->glbuf);
	check_error();
}

// }}}
// Geometry {{{

void
esGeoReset(esGeo *geo, int bufcount)
{
	geo->bufcount = bufcount;
}

void
esGeoPoint(esGeo *geo, int id, esGeoBuf *geobuf,
		enum esGeoDataType datatype, int elements,
		size_t offset, size_t stride, enum esBool normalized)
{
	geo->buf[id].geobuf = geobuf;
	geo->buf[id].datatype = datatype;
	geo->buf[id].elements = elements;
	geo->buf[id].offset = offset;
	geo->buf[id].stride = stride;
	geo->buf[id].normalized = normalized;
}

void
esGeoRender(const esGeo *geo, int vertices)
{
	static const unsigned int map[] = {
		[GEODATA_FLOAT] = GL_FLOAT,
		[GEODATA_INT] = GL_INT,
		[GEODATA_BYTE] = GL_BYTE,
		[GEODATA_UBYTE] = GL_UNSIGNED_BYTE,
	};

	int bufcount = geo->bufcount;
	const esGeoBuf *last = NULL;

	int i;
	for (i=0; i<bufcount; i++) {
		glEnableVertexAttribArray(i);

		if (last != geo->buf[i].geobuf) {
			last = geo->buf[i].geobuf;
			glBindBuffer(GL_ARRAY_BUFFER, last->glbuf);
		}

		glVertexAttribPointer(i,
				geo->buf[i].elements,
				map[geo->buf[i].datatype],
				geo->buf[i].normalized == ES_TRUE ? GL_TRUE : GL_FALSE,
				geo->buf[i].stride,
				(void*) geo->buf[i].offset);
		check_error();
	}

	glDrawArrays(GL_TRIANGLES, 0, vertices);
	check_error();

	for (i=0; i<bufcount; i++) glDisableVertexAttribArray(i);
	check_error();
}

// }}}
// Projection {{{

#define P0 (0.0f)
#define P1 (1.0f)

void
identity_matrix(float *mat)
{
	mat[ 0]=P1; mat[ 4]=P0; mat[ 8]=P0; mat[12]=P0;
	mat[ 1]=P0; mat[ 5]=P1; mat[ 9]=P0; mat[13]=P0;
	mat[ 2]=P0; mat[ 6]=P0; mat[10]=P1; mat[14]=P0;
	mat[ 3]=P0; mat[ 7]=P0; mat[11]=P0; mat[15]=P1;
}

static void
perspective_matrix(float *mat, float fov, float screenratio, float near, float far)
{
	float size = near * tanf(fov * 0.5); 
	float left = -size;
	float right = size;
	float bottom = -size / screenratio;
	float top = size / screenratio;

	mat[0] = 2 * near / (right - left);
	mat[1] = 0.0;
	mat[2] = 0.0;
	mat[3] = 0.0;

	mat[4] = 0.0;
	mat[5] = 2 * near / (top - bottom);
	mat[6] = 0.0;
	mat[7] = 0.0;

	mat[8] = (right + left) / (right - left);
	mat[9] = (top + bottom) / (top - bottom);
	mat[10] = -(far + near) / (far - near);
	mat[11] = -1;

	mat[12] = 0.0;
	mat[13] = 0.0;
	mat[14] = -(2 * far * near) / (far - near);
	mat[15] = 0.0;
}


static esVec3
cross(esVec3 a, esVec3 b)
{
	return (esVec3) {
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x,
	};
}

static void
normalize(esVec3 *v)
{
    float r;

    r = sqrtf(v->x*v->x + v->y*v->y + v->z*v->z);
    if (r == 0.0f) return;

	r = 1.0f / r;
    v->x *= r;
    v->y *= r;
    v->z *= r;
}

/*
static float
dot(esVec3 a, esVec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}*/

void
mul_matrix(float *res, float *a, float *b)
{
	int x, y, i=0;
	for (y=0; y<4; y++) {
		for (x=0; x<4; x++) {

			int r = y<<2;
			res[i] =
				a[r+ 0]*b[x+ 0] +
				a[r+ 1]*b[x+ 4] +
				a[r+ 2]*b[x+ 8] +
				a[r+ 3]*b[x+12];

			i++;
		}
	}
}

void
lookat_matrix(float *mat, esVec3 eye, esVec3 at, esVec3 up)
{
	esVec3 forw = {
		at.x - eye.x,
		at.y - eye.y,
		at.z - eye.z,
	};

	normalize(&forw);
	esVec3 side = cross(forw, up);
	normalize(&side);

	up = cross(side, forw);

	float m0[16];
	identity_matrix(m0);

	m0[ 0] = side.x;
	m0[ 4] = side.y;
	m0[ 8] = side.z;

	m0[ 1] = up.x;
	m0[ 5] = up.y;
	m0[ 9] = up.z;

	m0[ 2] = -forw.x;
	m0[ 6] = -forw.y;
	m0[10] = -forw.z;

	float m1[16];
	identity_matrix(m1);
	/*
	m1[12] = -dot(side, eye);
	m1[13] = -dot(up, eye);
	m1[14] = -dot(forw, eye);
	identity_matrix(m1);*/

	m1[12] = -eye.x;
	m1[13] = -eye.y;
	m1[14] = -eye.z;

	mul_matrix(mat, m0, m1);
}

void
esProjPerspective(
		float *mat, float fov, float screenratio, float near, float far,
		esVec3 eye, esVec3 at, esVec3 up)
{
	float persp[16];
	perspective_matrix(persp, fov, screenratio, near, far);
	//perspective_matrix(mat, fov, screenratio, near, far);

	float look[16];
	lookat_matrix(look, eye, at, up);
	lookat_matrix(mat, eye, at, up);

	//mul_matrix(mat, look, persp);
}

// }}}
// Texture {{{

int
esTextureLoad(esTexture *tex, const char *file_name,
		enum esTextureMipmap min, enum esTextureMipmap mag)
{
	SDL_Surface *surf = IMG_Load(file_name);
	if (surf == NULL) return 1;

	tex->w = surf->w;
	tex->h = surf->h;

	GLuint gltex;
	glGenTextures(1, &gltex);
	tex->gltexture = gltex;
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);

	int mode = surf->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h,
			0, mode, GL_UNSIGNED_BYTE, surf->pixels);

	static const GLenum map[] = {
		[TEX_NONE] = GL_NEAREST,
		[TEX_LINEAR] = GL_LINEAR,
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, map[min]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, map[mag]);

	SDL_FreeSurface(surf);
	return 0;
}

void
esTextureUse(esTexture *tex)
{
	glBindTexture(GL_TEXTURE_2D, tex->gltexture);
}

void
esTextureUnload(esTexture *tex)
{
	GLuint gltex = tex->gltexture;
	glDeleteTextures(1, &gltex);
}

// }}}

