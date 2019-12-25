#version 330 core

out vec4 color;
in vec3 fragmentColor;

uniform float transparency;

void main() {
	color = vec4(fragmentColor, 1);
	//color = vec4(1, 0, 0, 1);
}