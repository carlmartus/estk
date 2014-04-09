#version 100
precision mediump float;

attribute vec3 in_vertex;
attribute vec4 in_col;

varying vec4 va_col;

void main()
{
	va_col = in_col;
	gl_Position = vec4(in_vertex, 1.0);
}

