#version 100
precision mediump float;

attribute vec3 in_vertex;
attribute vec2 in_uv;
attribute vec4 in_col;

varying vec2 va_uv;
varying vec4 va_col;

void main()
{
	va_uv = in_uv;
	va_col = in_col;
	gl_Position = vec4(in_vertex, 1.0);
}

