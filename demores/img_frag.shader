#version 100
precision mediump float;

varying vec2 va_uv;
varying vec4 va_col;

uniform sampler2D un_tex0;

void main()
{
	gl_FragColor = texture2D(un_tex0, va_uv) * va_col;
}

