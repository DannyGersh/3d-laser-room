#pragma once
#include "Debug.h"
#define MIN 0.0005
#define MAX 1000000

using namespace glm;
struct Face;


struct Eplane
{
	vec3 a, b, c, n;
	float d;

	Eplane(vec3 _a, vec3 _b, vec3 _c) :
		a(_a), b(_b), c(_c)
	{
		n = glm::cross(b - a, c - a);
		d = n.x * (-a.x) + n.y * (-a.y) + n.z * (-a.z);
	}
	Eplane() = default;
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
	// 4 = error

	static const int _NOintersection = 0;
	static const int _INtriangle = 1;
	static const int _Onedge = 2;
	static const int _Onvertex = 3;
	static const int _error = 4;
};



struct Line 
{
	vec3 a, b;

	bool isONline(vec3 p)
	{
		vec3 n = cross(a - b, a - p);
		if (
			n.x	> -MIN && n.x < MIN	&&
			n.y > -MIN && n.y < MIN	&&
			n.z > -MIN && n.z < MIN	 
			)
			return true;
		else
			return false;
		
		//if (
		//	ab.x < ap.x + MIN && ab.x > ap.x - MIN &&
		//	ab.y < ap.y + MIN && ab.x > ap.y - MIN &&
		//	ab.z < ap.z + MIN && ab.x > ap.z - MIN
		//	)
		//vec3 ab = normalize(a - b);
		//vec3 ap = normalize(a - p);
		//if (ab == ap)
		//	return true;
		//
		//else
		//	return false;
	}

	bool operator==(Line l) 
	{
		if ((a == l.a && b == l.b)||(a == l.b && b == l.a)) {
			return true;
		}
		else {
			return false;
		}
	}
	void operator=(Line l)
	{
		a = l.a;
		b = l.b;
	}

};

struct edge
{
	Face* face[2];
	Line l;

	vec3 n;

};

struct Face 
{
	vec3 n;
	vec3 p[3];
	Line l[3];	
	Eplane eplane;
	vector<edge> e;

	Face(vec3 a, vec3 b, vec3 c) {
		p[0] = a; p[1] = b; p[2] = c;
		n = cross(p[1] - p[0], p[2] - p[0]);
		eplane = Eplane(a, b, c);
	}
	Face() = default;

	void init_Equation() {
		eplane.a = p[0];
		eplane.b = p[1];
		eplane.c = p[2];
		eplane.n = n;
		eplane.d = n.x * (-p[0].x) + n.y * (-p[0].y) + n.z * (-p[0].z);
	}

	Line EdgePoint_Intersect(vec3 p)
	{
		for (int i = 0; i < 3; i++)
		{
			if (l[i].isONline(p)) {
				return l[i];
			}
		}
	}
	edge search_edge_intersect(vec3 p) {
		for (auto _e: e) {
			if (_e.l.isONline(p)) {
				return _e;
			}
		}
	}

	bool operator == (Face& f)
	{
		if (
			p[0] == f.p[0] &&
			p[1] == f.p[1] &&
			p[2] == f.p[2]
			)
			return true;
		else
			return false;
	}
	bool operator != (Face& f)
	{
		if (
			p[0] == f.p[0] &&
			p[1] == f.p[1] &&
			p[2] == f.p[2]
			)
			return false;
		else
			return true;
	}

};




vec3 C() {
	float r = static_cast<float> (rand()) / static_cast <float>(RAND_MAX);
	float g = static_cast<float> (rand()) / static_cast <float>(RAND_MAX);
	float b = static_cast<float> (rand()) / static_cast <float>(RAND_MAX);
	return { r,g,b };
}

vec3 C0{ 1,1,1 };
vec3 C1{ .3,.1,.1 };
vec3 C2{ .1,.3,.1 };
vec3 C3{ .1,.1,.3 };

vec3 Cr{ 1,0,0 };
vec3 Cg{ 0,1,0 };
vec3 Cb{ 0,0,1 };

