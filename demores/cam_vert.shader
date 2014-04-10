#version 100
precision mediump float;

attribute vec3 in_vertex;

uniform mat4 un_view;

void main()
{
	gl_Position = un_view*vec4(in_vertex, 1.0);
}

