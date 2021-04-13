#version 330 core

// fragment shader

in vec4 fragmentColor;
out vec4 color;

//uniform vec4 poop;

void main() {
	color = fragmentColor;
	//color = vec4(1., 0., 0., 1.);
};