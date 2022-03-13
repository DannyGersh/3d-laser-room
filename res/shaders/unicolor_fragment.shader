#version 330 core

out vec4 color;
in vec4 fragmentColor;
in vec4 pos;

uniform vec4 inCOLOR;
uniform vec2 iResolution;

void main() {
	color = inCOLOR;
	//if(gl_FragCoord.z-.5 < 0.) color = vec4(.0,.0,.0,.0);
}