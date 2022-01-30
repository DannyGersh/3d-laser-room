#version 330 core

out vec4 color;
in vec4 fragmentColor;
in vec4 pos;

uniform vec4 inCOLOR;
uniform vec2 iResolution;

void main() {
	color = inCOLOR;
}