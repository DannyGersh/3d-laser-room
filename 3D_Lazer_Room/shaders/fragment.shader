#version 330 core

out vec4 color;
in vec4 fragmentColor;

uniform float transparency;

void main() {
	color = fragmentColor;
	//color = vec4(1, 0, 0, 1);
}