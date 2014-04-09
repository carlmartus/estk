#version 100
precision mediump float;

varying vec4 va_col;

void main()
{
	gl_FragColor = va_col;
}

