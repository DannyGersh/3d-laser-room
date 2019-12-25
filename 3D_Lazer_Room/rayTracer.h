#pragma once
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

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
};

vec3 GETintersection(Eplane p, Eline l)
{
	float raw_d =
		p.n.x * l.point.x +
		p.n.y * l.point.y +
		p.n.z * l.point.z +
		p.d;

	float raw_t =
		p.n.x * l.t.x +
		p.n.y * l.t.y +
		p.n.z * l.t.z;

	float t = -raw_d / raw_t;

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
bool isINtriangle(Eplane p, vec3 i)
{
	float abc = triangleAREA(p.a, p.b, p.c);
	float abp = triangleAREA(p.a, p.b, i);
	float acp = triangleAREA(p.a, p.c, i);
	float cbp = triangleAREA(p.c, p.b, i);
	float _abc = abp + acp + cbp;

	if (_abc > abc - .00001 && _abc < abc + .00001)
	{
		return true;
	}
	return false;
}

vec3 reflect(Eplane p, vec3 d)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 n = normalize(p.n);
	return { d - vec3{2,2,2} *dot(n, d) * n };
}



Ray wanWAYray(vec3 Ta, vec3 Tb, vec3 Tc, vec3 La, vec3 Lb)
{
	Eplane p(Ta, Tb, Tc);
	Eline l(La, Lb);

	vec3 intersect = GETintersection(p, l);
	vec3 direction = reflect(p, intersect) + intersect;

	if (isINtriangle(p, intersect))
	{
		vec3 _a{ normalize(La - Lb) };
		vec3 _b{ normalize(La - intersect) };
		if (
			_a.x < _b.x + 0.1 &&
			_a.x > _b.x - 0.1 &&
			_a.y < _b.y + 0.1 &&
			_a.y > _b.y - 0.1 &&
			_a.z < _b.z + 0.1 &&
			_a.z > _b.z - 0.1
			)
		{
			return{ intersect, direction };
		}
	}
	return{ {0,0,0},{0,0,0} };

}





