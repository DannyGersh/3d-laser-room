//#version 330 core
//
//layout(location = 0) in vec3 v_pos;
//layout(location = 1) in vec4 v_color;
//
//layout (std140) uniform ColorBlock {
//    vec3 camTranslation;
//	mat3 camRotation;
//	//uniform vec2 iResolution;
//};
//
//out vec4 fragmentColor;
//
//uniform mat3 trans;
//uniform vec2 iMouse;
//uniform vec3 loc;
//uniform vec2 iResolution;
//
//void main() 
//{
//	vec3 p = camRotation * (trans * v_pos + loc) + camTranslation;
//
//	if( iResolution.x > iResolution.y ) p.x /= iResolution.x / iResolution.y;
//	else p.y /= iResolution.y / iResolution.x;
//		
//	// track mouse
//	//p = p + vec3(iMouse.xy, .0);
//	
//	// projection
//	float q = -(p.z / 2.); // divisor here is the distance to the vanishing point
//	float x = q * p.x + p.x;
//	float y = q * p.y + p.y;
//
//	gl_Position = vec4( x, y, p.z, 1.0 );
//	fragmentColor = v_color;
//};




#version 330 core

layout(location = 0) in vec3 v_pos;
layout(location = 1) in vec4 v_color;

layout (std140) uniform GlobalUniform {
    vec3 camTranslation;
	vec4 iResolution;
	vec4 iMouse;
	mat3 camRotation;
};

out vec4 fragmentColor;

uniform mat3 trans;
uniform vec3 loc;

void main() 
{
	//vec3 p = camRotation * (trans * v_pos + loc) + camTranslation;
	//vec3 p = camRotation * (trans * v_pos + loc) + vec3(iResolution, .0);
	vec3 p = trans * v_pos + loc;
	p += vec3(.0, .0, 2.);
	p = camRotation * p;
	p -= vec3(.0, .0, 2.);
	p += camTranslation;
	
	if(p.z>3.) p.z = 3.;
	
	if( iResolution.x > iResolution.y ) p.x /= iResolution.x / iResolution.y;
	else p.y /= iResolution.y / iResolution.x;
		
	// track mouse
	// p = p - vec3(iMouse.xy, .0);
	
	// projection
	float q = -(p.z / 3.); // divisor here is the distance to the vanishing point
	float x = q * p.x + p.x;
	float y = q * p.y + p.y;

	gl_Position = vec4( x, y, p.z, 1.0 );
	fragmentColor = v_color;
};



