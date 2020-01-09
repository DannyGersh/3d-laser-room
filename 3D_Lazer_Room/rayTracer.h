#pragma once
#include "Debug.h"
#include "Primitives.h"

using namespace glm;
using namespace std;



inline vec4 vtv(vec3 v) {
	return { v.x,v.y,v.z,0 };
}
inline vec3 vtv(vec4 v) {
	return { v.x,v.y,v.z };
}

inline float v3v3DEGREE(vec3 a, vec3 b)
{
	return dot(a, b) / (length(a)*length(b));
}

inline bool sameDir(Line la, Line lb) {
	vec3 a{ normalize(la.a - la.b) };
	vec3 b{ normalize(lb.a - lb.b) };
	if (
		a.x > b.x - .001 && a.x < b.x + .001 &&
		a.y > b.y - .001 && a.y < b.y + .001 &&
		a.z > b.z - .001 && a.z < b.z + .001
		) 
		return true;

	return false;
}
inline bool sameDir(vec3 va, vec3 vb) {
	vec3 a{ normalize(va) };
	vec3 b{ normalize(vb) };
	if (
		a.x > b.x - MIN && a.x < b.x + MIN &&
		a.y > b.y - MIN && a.y < b.y + MIN &&
		a.z > b.z - MIN && a.z < b.z + MIN
		)
		return true;
	//vec3 a = cross(vb, va);
	//if (
	//	a.x > -MIN && a.x < MIN &&
	//	a.y > -MIN && a.y < MIN &&
	//	a.z > -MIN && a.z < MIN
	//	) return true;
	//
	return false;
}

inline vec3 GETintersection(Eplane& p, Eline l)
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

inline int testRayTriangle(vec3& a, vec3& b, vec3& c, vec3& p)
{
	const float e1 = dot(cross(a, b), p);
	const float e2 = dot(cross(b, c), p);
	const float e3 = dot(cross(c, a), p);

	if (e1 < 0 || e2 < 0 || e3 < 0) {
		return Ray::_NOintersection;
	}
	else if (
		(e1 > -MIN && e1 < MIN) ||
		(e2 > -MIN && e2 < MIN) ||
		(e3 > -MIN && e3 < MIN)
		) {
		q("_omEDGE");
		q(e1); q(e2); q(e3); q();
		return Ray::_Onedge;
	}
	else {
		q("_INtriangle");
		q(e1); q(e2); q(e3); q();
		return Ray::_INtriangle;
	}
}


inline vec3 reflect(Eplane plane, vec3 direction)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 normal = normalize(plane.n);
	return { direction - (vec3{2,2,2} * dot(normal, direction) * normal) };
}
inline vec3 reflect(vec3 n, vec3 direction)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 normal = normalize(n);
	return { direction - (vec3{2,2,2} *dot(normal, direction) * normal) };
}

void rayTRIANGLEintersect(Line& line, vec3& intersect, vec3* normal, Face& f, int* last, float& l)
{

	vec3 _intersect = GETintersection(f.eplane, Eline{ line.a, line.b });

	if (sameDir(line.a - line.b, line.a - _intersect))
	{
		int r = testRayTriangle(f.p[0], f.p[1], f.p[2], _intersect);

		if (r == Ray::_Onedge) {
			float ll = length(line.a - _intersect);
			if (ll < l) { l = ll; intersect = _intersect; }

			vec3 n[2]; Face fff[2];
			for (int i = 0; i < 3; i++) {
				if (f.e[i].l.isONline(intersect)) {
					n[0] = f.e[i].face[0]->n;
					n[1] = f.e[i].face[1]->n;
				}
			}
			//if (v3v3DEGREE(line.a - _intersect, n[0]) < v3v3DEGREE(line.a - _intersect, n[1])) {
			//	normal = n[0];
			//}
			//else {
			//	normal = n[1];
			//}
			//normal = n[0] + n[1];

			if (!sameDir(normalize(n[0]), normalize(n[1]))) {
				 normal[0] = n[0]; normal[1] = n[1];
				 *last = Ray::_Onedge;
			}
			else {
				normal[0] = n[0]; 
				*last = Ray::_INtriangle;
			}
			
		}
		else if (r == Ray::_INtriangle) {
			float ll = length(line.a - _intersect);
			if (ll < l) { l = ll; intersect = _intersect; normal[0] = f.n; }
			*last = Ray::_INtriangle;
		}
		
	}
}







