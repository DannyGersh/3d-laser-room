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
int isINtriangle(Eplane p, vec3 i)
{
	float abc = triangleAREA(p.a, p.b, p.c);

	float abp = triangleAREA(p.a, p.b, i);
	float acp = triangleAREA(p.a, p.c, i);
	float cbp = triangleAREA(p.c, p.b, i);
	float _abc = abp + acp + cbp;

	if (_abc > abc - .00001 && _abc < abc + .00001)
	{
		return 1;
	}
	//if (
	//	(abp + acp > 0 && cbp < 0.01) ||
	//	(abp + cbp > 0 && acp < 0.01) ||
	//	(acp + cbp > 0 && abp < 0.01)
	//	)
	//{
	//	return 2;
	//}
	//else if (
	//	abp == 0 && acp == 0 ||
	//	acp == 0 && cbp == 0 ||
	//	abp == 0 && cbp == 0
	//	)
	//{
	//	return 3;
	//}

	return false;
}

vec3 reflect(Eplane plane, vec3 direction)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 normal = normalize(plane.n);
	return { direction - vec3{2,2,2} * dot(normal, direction) * normal };
}



Ray wanWAYray(vec3 Ta, vec3 Tb, vec3 Tc, vec3 La, vec3 Lb)
{
	Eplane p(Ta, Tb, Tc);
	Eline l(La, Lb);

	vec3 intersect = GETintersection(p, l);
	vec3 direction = reflect(p, l.t);

	int result = isINtriangle(p, intersect);

	if (result)
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
		{
			if (result == 1) {
				return{ intersect, direction };
			}
			if (result == 2) {
				//wxMessageBox("POOP 2");
				return{ intersect, {.5,.1,.1} };
			}
			if (result == 3) {
				//wxMessageBox("POOP 3");
				return{ intersect, {.5,.1,.1} };
			}
		}
	}
	
	return{ {0,0,0},{0,0,0} };

}





