#pragma once
#include "Primitives.h"


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

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}
	else {
		wxLogError("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}