#version 100
precision mediump float;

attribute vec3 in_vertex;

uniform vec4 un_base;

varying vec4 va_base;

void main()
{
	va_base = un_base;
	gl_Position = vec4(in_vertex, 1.0);
}

