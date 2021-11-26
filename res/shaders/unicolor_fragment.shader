#version 330 core

out vec4 color;
in vec4 fragmentColor;

uniform vec4 inCOLOR;

void main() {
	color = inCOLOR;
	color = vec4(0, 1, 1, 1);
}