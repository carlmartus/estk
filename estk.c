// Top {{{
#include "estk.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <GL/glew.h>

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
check_shader(GLuint id)
{
	GLint result = GL_FALSE;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result != GL_TRUE) {
		char info_buf[500];
		glGetShaderInfoLog(id, sizeof(info_buf)-1, NULL, info_buf);
		printf("Shader info:\n%s\n", info_buf);
		check_error();
		return 1;
	}

	return 0;
}

int
shader_load(const char *file_name, GLenum shader_type)
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

	if (check_shader(shad)) return 0;

	return shad;
}

int
esShaderLoad(esShader *shader, const char *vert_file, const char *frag_file)
{
	int idvert = shader_load(vert_file, GL_VERTEX_SHADER);
	if (idvert == 0) return 1;
	int idfrag = shader_load(frag_file, GL_FRAGMENT_SHADER);
	if (idfrag == 0) return 1;

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

