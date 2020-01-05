#version 330 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
out vec4 fragmentColor;

uniform mat4 trans;

void main() {
	gl_Position = trans * vec4(v_pos, 1.0);
	//gl_Position = vec4(v_pos, 1.0);

	fragmentColor = v_color;
};
