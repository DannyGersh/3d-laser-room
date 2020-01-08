#pragma once
#include "Primitives.h"


GLuint compileSHADER(const char * file_path, int shaderTYPE)
{
	// GL_VERTEX_SHADER // GL_FRAGMENT_SHADER

	GLuint ShaderID = glCreateShader(shaderTYPE);
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(ShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(ShaderID);
	return ShaderID;
}
void linkPROGRAM(GLuint programID, GLuint vectSHADER, GLuint fragSHADER)
{
	glAttachShader(programID, vectSHADER);
	glAttachShader(programID, fragSHADER);
	glLinkProgram(programID);

	glDetachShader(programID, vectSHADER);
	glDetachShader(programID, fragSHADER);

	glUseProgram(programID);
	// glDeleteShader(vert);
	// glDeleteShader(frag);
}
void sendUNIFORMdata(GLuint programID, mat4 mat)
{
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}
void set_uniCOLOR(GLuint _uniCOLOR_programID, glm::vec4 color)
{
	GLint uniTrans = glGetUniformLocation(_uniCOLOR_programID, "inCOLOR");
	glUniform4fv(uniTrans, 1, &color[0]);
}

vec3 rotate(vec3 p, vec3 degree)
{
	mat4 lazer(1);
	vec4 p4 = vtv(p);
	p4 = p4 * glm::rotate(lazer, float(radians(degree.x)), vec3{ 0,0,1 });
	p4 = p4 * glm::rotate(lazer, float(radians(degree.y)), vec3{ 0,1,0 });
	p4 = p4 * glm::rotate(lazer, float(radians(degree.z)), vec3{ 0,0,1 });
	return vtv(p4);
}
void rotate(GLuint programID, glm::mat4& mat, float degree, glm::vec3 axis)
{
	mat = glm::rotate(mat, glm::radians(degree), axis); // rotate 
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}
void scale(GLuint programID, glm::mat4& mat, glm::vec3 scale)
{
	mat = glm::scale(mat, scale);
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}
void move(GLuint programID, glm::mat4& mat, glm::vec3 scale)
{
	mat = glm::translate(mat, scale);
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}



void drawLINEs(vector<Line> lines)
{	
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, lines.capacity() * sizeof(Line) + sizeof(lines), &lines[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(vec3),               // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glDrawArrays(GL_LINES, 0, lines.size()*2);
}

void drawLINE(vec3 a, vec3 b, vec4 color)
{
	struct data {
		vec3 point;
		vec4 color;
	};
	data d[] = {
		{a, color},
		{b, color},
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(d), &d, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(data),               // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct data),
		(void*)(sizeof(data::point))
	);
	glDrawArrays(GL_LINES, 0, 3);
}
void drawLINE(Line l, vec4 color)
{
	struct data {
		vec3 point;
		vec4 color;
	};
	data d[] = {
		{l.a, color},
		{l.b, color},
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(d), &d, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(data),               // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct data),
		(void*)(sizeof(data::point))
	);
	glDrawArrays(GL_LINES, 0, 3);
}

void drawLINE2(vec3 a, vec3 b, vec4 color)
{
	struct data {
		vec3 point;
		vec4 color;
	};
	data d[] = {
		{a - vec3{.01,.01,.01}, color},
		{b - vec3{.01,.01,.01}, color},
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(d), &d, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(data),               // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct data),
		(void*)(sizeof(data::point))
	);
	glDrawArrays(GL_LINES, 0, 3);
}
void drawLINE2(Line l, vec4 color)
{
	struct data {
		vec3 point;
		vec4 color;
	};
	data d[] = {
		{l.a, color},
		{l.b, color},
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(d), &d, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(data),               // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct data),
		(void*)(sizeof(data::point))
	);
	glDrawArrays(GL_LINES, 0, 3);
}

void drawTriangle(vec3 a, vec3 b, vec3 c, vec4 color)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	struct attributes {
		glm::vec3 position;
		glm::vec4 _color;
	};
	attributes triangle_attributes[] = {
		{a, color},
		{b, color},
		{c, color},
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(struct attributes),  // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct attributes),
		(void*)(sizeof(attributes::position))
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
void drawTriangle(Face f, vec4 color)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	struct attributes {
		glm::vec3 position;
		glm::vec4 _color;
	};
	attributes triangle_attributes[] = {
		{f.p[0], color},
		{f.p[1], color},
		{f.p[2], color},
	};

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(struct attributes),  // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(struct attributes),
		(void*)(sizeof(attributes::position))
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}



