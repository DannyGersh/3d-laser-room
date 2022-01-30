#version 330 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;
out vec4 fragmentColor;

uniform mat3 trans;
uniform vec2 iMouse;
uniform vec2 iResolution;

void main() 
{
	vec3 p = trans * v_pos;
	if( iResolution.x > iResolution.y ) 
	{
		float r = iResolution.x / iResolution.y;
		p.x /= r;
	}
	else 
	{
		float r = iResolution.y / iResolution.x;
		p.y /= r;
	}
	p = p + vec3(iMouse.xy, .0);
	
	// projection
	float q = -(p.z / 2.); // divisor here is the distance to the vanishing point
	float x = q * p.x + p.x;
	float y = q * p.y + p.y;

	gl_Position = vec4( x, y, p.z, 1.0 );
	fragmentColor = v_color;
};

