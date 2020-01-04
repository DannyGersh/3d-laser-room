#pragma once
#include "Debug.h"
#include "Primitives.h"

using namespace glm;
using namespace std;



vec4 vtv(vec3 v) {
	return { v.x,v.y,v.z,0 };
}
vec3 vtv(vec4 v) {
	return { v.x,v.y,v.z };
}

struct RayState {
	vec3 intersect;
	vec3 weights;
	int state;
};

bool sameDir(Line la, Line lb) {
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
bool sameDir(vec3 va, vec3 vb) {
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

inline vec3 GETintersection(Eplane p, Eline l)
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



inline float triangleAREA(vec3 a, vec3 b, vec3 c)
{
	return(length(cross(b - a, c - a)) / 2);
}
inline int isINtriangle3(vec3 a, vec3 b, vec3 c, vec3 p)
{
	float area_abc = length(cross(a - b, a - c)) / 2;
	float area_0 = length(cross(b - c, b - p)) / 2;
	float area_1 = length(cross(c - a, c - p)) / 2;
	float area_2 = length(cross(a - b, a - p)) / 2;

	float u = area_0 / area_abc;
	float v = area_1 / area_abc;
	float w = area_2 / area_abc;

	if (
		(u == 0 && v + w > 0) || 
		(v == 0 && u + w > 0) || 
		(w == 0 && u + v > 0))
	{
		return Ray::_Onedge;
	}
	else if (
		0 <= u && u <= 1 &&
		0 <= v && v <= 1 &&
		0 <= w && w <= 1 &&
		u + v + w == 1
		)
		return Ray::_INtriangle;
	
	else
		Ray::_NOintersection;

}

inline int _isINtriangle(vec3 a, vec3 b, vec3 c, vec3 p)
{
	float total_area = triangleAREA(a, b, c);
	float e1 = triangleAREA(a, b, p);
	float e2 = triangleAREA(a, c, p);
	float e3 = triangleAREA(c, b, p);
	float t = e1 + e2 + e3;

	if (
		(e1 > -MIN && e1 < MIN) ||
		(e2 > -MIN && e2 < MIN) ||
		(e3 > -MIN && e3 < MIN)
		)
	{
		qq(e1); qq(e2); qq(e3); qq();
		return Ray::_Onedge;
	}
	else if (e1 < 0 || e2 < 0 || e3 < 0) {
		return Ray::_NOintersection;
	}
	else if (
		e1 > MIN && e2 > MIN && e3 > MIN &&
		e1 < MAX && e2 < MAX && e3 < MAX
		)
	{
		qq(e1); qq(e2); qq(e3); qq();
		return Ray::_INtriangle;
	}

}

inline int _isINtriangle2(vec3 a, vec3 b, vec3 c, vec3 p)
{
	vec3 N = cross(a, b);
	vec3 abp = cross(b - a, a - p);
	vec3 acp = cross(c - a, a - p);
	vec3 bcp = cross(c - b, c - p);

	float u = dot(N, abp);
	float v = dot(N, acp);
	float w = dot(N, bcp);

	if (u < 0 && v < 0 && w < 0) 
	{
		return Ray::_INtriangle;
	}
	return Ray::_NOintersection;
}

inline  int _isINtriangle3(vec3 a, vec3 b, vec3 c, vec3 p) {
	float raw_a = (b.y - c.y)*(p.x - c.x) + (c.x - b.x)*(p.y - c.y);
	float raw_b = (b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y);
	float u = raw_a / raw_b;

	raw_a = (c.y - a.y)*(p.x - c.x) + (a.x - c.x)*(p.y - c.y);
	raw_b = (b.y - c.y)*(a.x - c.x) + (c.x - b.x)*(a.y - c.y);
	float v = raw_a / raw_b;

	float w = 1 - u - v;
	
	if (u == 0 && v > 0 && w > 0 || 
		v == 0 && u > 0 && w > 0 ||
		w == 0 && u > 0 && v > 0
		) {
		return Ray::_Onedge;
	}
	//else if (u > 0.0001 && v > 0.0001 && w > 0.0001) {
	//	return Ray::_INtriangle;
	//}
	else {
		return Ray::_NOintersection;
	}
}




// 0 when [0,p] ray goes through [0,t1,t2] plane.
// Otherwise, the sign tells relative orientation of the ray and the plane.
// 0 when [0,p] ray goes through [0,t1,t2] plane.
// Otherwise, the sign tells relative orientation of the ray and the plane.
inline float crossDot(vec3 t1, vec3 t2, vec3 p)
{
	// Depending on winding order of triangles, and coordinate system (right versus left handed),
	// you may need to flip the cross() arguments
	return dot(cross(t1, t2), p);
}

// The triangle is [a, b, c], the points must have consistent winding,
// i.e. when viewed from [0,0,0] the order must be either clockwise or counterclockwise,
// for all triangles of your mesh.
// p does not need to be normalized, length is ignored, only direction is used.
int testRayTriangle(vec3 a, vec3 b, vec3 c, vec3 p)
{
	// If this assert fails because zero, you have a zero-area triangle, or a triangle that goes through [0,0,0].
	// If this assert fails because negative, you need to flip cross() arguments, or fix winding order in the mesh.
	assert(crossDot(a, b, c) > 0);
	const float e1 = crossDot(a, b, p);
	const float e2 = crossDot(b, c, p);
	const float e3 = crossDot(c, a, p);

	if (e1 < 0 || e2 < 0 || e3 < 0)
		return Ray::_NOintersection;

	else if (
		(e1 > -MIN && e1 < MIN) ||
		(e2 > -MIN && e2 < MIN) ||
		(e3 > -MIN && e3 < MIN)
		)
		return Ray::_Onedge;

	else
		return Ray::_INtriangle;
}



int isINtriangle500(vec3 a, vec3 b, vec3 c, vec3 p)
{

	vec3 x = { a.x,b.x,c.x };
	vec3 y = { a.y,b.y,c.y };
	vec3 z = { a.z,b.z,c.z };
	float X{ 0 }; float Y{ 0 }; float Z{ 0 };

	//if (
	//	x.x + y.x == 0 && x.y + y.y == 0 ||
	//	x.x + y.x == 0 && x.z + y.z == 0 ||
	//	x.y + y.y == 0 && x.z + y.z == 0
	//	)
	//	float v = (p.x + p.y) / ( (x.x + y.x) + (x.y + y.y) + (x.z + y.z) );
	//if (
	//	x.x - y.x == 0 && x.y - y.y == 0 ||
	//	x.x - y.x == 0 && x.z - y.z == 0 ||
	//	x.y - y.y == 0 && x.z - y.z == 0
	//	)
	//	float v = (p.x - p.y) / ((x.x - y.x) + (x.y - y.y) + (x.z - y.z));
	//
	//
	//if (
	//	x.x + z.x == 0 && x.y + z.y == 0 ||
	//	x.x + z.x == 0 && x.z + z.z == 0 ||
	//	x.y + z.y == 0 && x.z + z.z == 0
	//	)
	//{};
	//if (
	//	y.x + z.x == 0 && y.y + z.y == 0 ||
	//	y.x + z.x == 0 && y.z + z.z == 0 ||
	//	y.y + z.y == 0 && y.z + z.z == 0
	//	)
	//{};

	if (x.x - z.x == 0 && x.y - z.y == 0)
		Y = (p.x - p.z) / (x.z - z.z);

	if (X == 0 && Z == 0) {

	}

	qq("__________");
	qq(Y);
	qq("__________");

	return 0;
}


vec3 reflect(Eplane plane, vec3 direction)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 normal = normalize(plane.n);
	return { direction - (vec3{2,2,2} * dot(normal, direction) * normal) };
}
vec3 reflect(vec3 n, vec3 direction)
{
	// I - 2.0 * dot(N, I) * N.

	vec3 normal = normalize(n);
	return { direction - (vec3{2,2,2} *dot(normal, direction) * normal) };
}


Ray wanWAYray(vec3 Ta, vec3 Tb, vec3 Tc, vec3 La, vec3 Lb)
{
	Eplane p(Ta, Tb, Tc);
	Eline l(La, Lb);

	vec3 intersect = GETintersection(p, l);
	vec3 direction = glm::reflect(l.t, normalize(p.n));

	//RayState state = isINtriangle2(p, intersect);

	//if (state.state != Ray::_NOintersection)
	//{ 
	//	if (state.state != Ray::_error) { 
	//		q("Weights: "); q(state.weights); q();
	//		q("Intersect: "); q(state.intersect);
	//		return { intersect, direction, state.state };
	//	}
	//}
	//return{ {0,0,0},{0,0,0}, Ray::_NOintersection };
	

}







