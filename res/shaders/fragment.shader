#version 330 core

out vec4 color;
in vec4 fragmentColor;

uniform vec4 poop;

void main() {
	color = fragmentColor;
	//color = vec4(1, 0, 0, 1);
}