#pragma once
#include <string>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

//#define DEBUG

string str{ "" };

#ifdef _DEBUG
void q() {
	str += "\n";
}
void q(size_t _str) {
	str += to_string(_str) + '\n';
}
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
#else
void q() {}
void q(size_t _str) {}
void q(string _str) {}
void q(float _str) {}
void q(int _str) {}
void q(vec3 a) {}
void q(vec3 a, vec3 b, vec3 c) {}
#endif // DEBUG

void qq() {
	str += "\n";
}
void qq(size_t _str) {
	str += to_string(_str) + '\n';
}
void qq(string _str) {
	str += _str + '\n';
}
void qq(float _str) {
	str += to_string(_str);
	str += '\n';
}
void qq(int _str) {
	str += to_string(_str);
	str += '\n';
}
void qq(vec3 a) {
	str += to_string(a.x) + '\n';
	str += to_string(a.y) + '\n';
	str += to_string(a.z) + '\n';
}
void qq(vec3 a, vec3 b, vec3 c) {
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
