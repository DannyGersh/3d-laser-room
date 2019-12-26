#pragma once
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

string str{ "" };
void q(string _str) {
	str += _str + '\n';
}
void q(float _str) {
	str += to_string(_str);
	str += '\n';
}
void q(int _str) {
	str += to_string(_str);
	str += '\n';
}
void q(vec3 a) {
	str += to_string(a.x) + '\n';
	str += to_string(a.y) + '\n';
	str += to_string(a.z) + '\n';
	str += "\n\n";
}
void q(vec3 a, vec3 b, vec3 c) {
	str += to_string(a.x) + '\n';
	str += to_string(a.y) + '\n';
	str += to_string(a.z) + '\n';
	str += "\n";
	str += to_string(b.x) + '\n';
	str += to_string(b.y) + '\n';
	str += to_string(b.z) + '\n';
	str += "\n";
	str += to_string(c.x) + '\n';
	str += to_string(c.y) + '\n';
	str += to_string(c.z) + '\n';
	str += "\n\n";
}

vec4 vtv(vec3 v) {
	return { v.x,v.y,v.z,0 };
}
vec3 vtv(vec4 v) {
	return { v.x,v.y,v.z };
}



struct Eplane
{
	vec3 a, b, c, n;
	float d;

	Eplane(vec3 a, vec3 b, vec3 c) :
		a(a), b(b), c(c)
	{
		n = glm::cross(b - a, c - a);
		d = n.x * (-a.x) + n.y * (-a.y) + n.z * (-a.z);
	}
};
struct Eline
{
	vec3 point, t;

	Eline(vec3 a, vec3 b)
	{
		point = a;
		t = b - a;
	}
};
struct Ray {
	vec3 intersect;
	vec3 direction;
	int state;

	// state:
	// 0 = intersection is NOT in triangle
	// 1 = intersection is in triangle
	// 2 = intersection is on triangle edge
	// 3 = intersection is on a vertex

	static const int _NOintersection = 0;
	static const int _INtriangle = 1;
	static const int _Onedge = 2;
	static const int _Onvertex = 3;
};

vec3 GETintersection(Eplane p, Eline l)
{
	long double raw_d =
		p.n.x * l.point.x +
		p.n.y * l.point.y +
		p.n.z * l.point.z +
		p.d;

	long double raw_t =
		p.n.x * l.t.x +
		p.n.y * l.t.y +
		p.n.z * l.t.z;

	long double t = -raw_d / raw_t;

	vec3 result{
		l.point.x + l.t.x * t,
		l.point.y + l.t.y * t,
		l.point.z + l.t.z * t
	};

	return result;
}

float triangleAREA(vec3 a, vec3 b, vec3 c)
{
	vec3 ab = b - a;
	vec3 ac = c - a;
	vec3 n = cross(ab, ac);
	return(sqrt(n.x*n.x + n.y*n.y + n.z*n.z) / 2);
}
int isINtriangle(Eplane p, vec3 i)
{
	// return value is the STATE of intersection
	
	float abc = triangleAREA(p.a, p.b, p.c);

	float a = triangleAREA(p.a, p.b, i);
	float b = triangleAREA(p.a, p.c, i);
	float c = triangleAREA(p.c, p.b, i);
	float _abc = a + b + c;

	if (_abc > abc - .0001 && _abc < abc + .0001)
	{
		if (
			(a + b > 0 && c == 0) |
			(a + c > 0 && b == 0) |
			(c + b > 0 && a == 0)
			)
			return Ray::_Onedge;
		
		else if (
			a == 0 && b == 0 ||
			a == 0 && c == 0 ||
			c == 0 && b == 0
			)
			return Ray::_Onvertex;
		
		else
			return Ray::_INtriangle;
	}

	return Ray::_NOintersection;
}

vec3 reflect(Eplane plane, vec3 direction)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 normal = normalize(plane.n);
	return { direction - (vec3{2,2,2} * dot(normal, direction) * normal) };
}



Ray wanWAYray(vec3 Ta, vec3 Tb, vec3 Tc, vec3 La, vec3 Lb)
{
	Eplane p(Ta, Tb, Tc);
	Eline l(La, Lb);

	vec3 intersect = GETintersection(p, l);
	vec3 direction = reflect(p, l.t);

	int state = isINtriangle(p, intersect);

	if (state != Ray::_NOintersection)
	{
		vec3 _a{ normalize(La - Lb) };
		vec3 _b{ normalize(La - intersect) };
		if // intersection is in the same direction as the ray
			( 
			_a.x < _b.x + 0.1 &&
			_a.x > _b.x - 0.1 &&
			_a.y < _b.y + 0.1 &&
			_a.y > _b.y - 0.1 &&
			_a.z < _b.z + 0.1 &&
			_a.z > _b.z - 0.1
			)
			return { intersect, direction, state };
	}
	
	return{ {0,0,0},{0,0,0}, 0 };

}





