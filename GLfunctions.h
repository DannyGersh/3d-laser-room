#pragma once
#include "pch.h"

namespace GL
{

void _CheckError(const char* file, int line)
{
	GLenum err = glGetError();
	std::string str;
	
	if(err!=GL_NO_ERROR)
	{		
		switch (err)
		{
		case GL_INVALID_ENUM:
			str += " - GLenum argument out of range\n";
		case GL_INVALID_VALUE:
			str += "- Numeric argument out of range.\n";
		case GL_INVALID_OPERATION:
			str += " - Operation illegal in current state.\n";
		case GL_STACK_OVERFLOW:
			str += " - Function would cause a stack overflow.\n";
		case GL_STACK_UNDERFLOW:
			str += " - Function would cause a stack underflow.\n";
		case GL_OUT_OF_MEMORY:
			str += " - Not enough memory left to execute function.\n";
		}
		
		str += std::string("\n") + file + std::string(": ") + std::to_string(line);
		wxLogError(str.c_str());
	}
}
#define CheckError() _CheckError(__FILE__, __LINE__)

GLuint CompileShader(const char * file_path, int shaderTYPE)
{
	// shaderTYPE: GL_VERTEX_SHADER , GL_FRAGMENT_SHADER

	GLuint ShaderID = glCreateShader(shaderTYPE);
	std::string ShaderCode;
	std::ifstream File(file_path, std::ios::in);
	
	assert(File.good());
	while(File.good()) ShaderCode += File.get();
	
	char const * code = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &code, NULL);
	glCompileShader(ShaderID);
	
	// debug
	{
		GLint Success = 0;
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Success);
		if(Success == GL_FALSE)
		{
			GLint LogSize = 0;
			glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &LogSize);
			
			char Log[LogSize];
			glGetShaderInfoLog(ShaderID, LogSize, &LogSize, &Log[0]);
			std::cout<<Log;
			assert(Success);
		}
		
		CheckError();
	}
	
	return ShaderID;
}
void LinkProgram(GLuint programID, GLuint SHvertex, GLuint SHfragment)
{
	glAttachShader(programID, SHvertex);
	glAttachShader(programID, SHfragment);
	glLinkProgram(programID);
	
	// debug
	{
		GLint Success = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, &Success);
		if(Success == GL_FALSE)
		{
			GLint LogSize = 0;
			glGetProgramiv (programID, GL_INFO_LOG_LENGTH, &LogSize);
			
			char Log[LogSize];
			glGetProgramInfoLog(programID, LogSize, &LogSize, &Log[0]);
			
			DB::LogT(Log);
			assert(Success);
		}
		
		CheckError();
	}
	
	glDetachShader(programID, SHvertex);
	glDetachShader(programID, SHfragment);
	
	glUseProgram(programID);
	
	glDeleteShader(SHvertex);
	glDeleteShader(SHfragment);
	
	CheckError();
}

void SetUniform(GLuint programID, glm::mat4 mat, const char* var)
{
	// var - name of variable to be set in shader
	GLint uniTrans = glGetUniformLocation(programID, var);
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat));
	CheckError();
}
void SetUniform(GLuint programID, glm::mat3 mat, const char* var)
{
	// var - name of variable to be set in shader
	GLint uniTrans = glGetUniformLocation(programID, var);
	glUniformMatrix3fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat));
	CheckError();
}
void SetUniform(GLuint programID, glm::mat2 mat, const char* var)
{
	// var - name of variable to be set in shader
	GLint uniTrans = glGetUniformLocation(programID, var);
	glUniformMatrix2fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat));
	CheckError();
}
void SetUniform(GLuint programID, glm::vec4 color, const char* var)
{
	// var - name of variable to be set in shader
	GLint uniTrans = glGetUniformLocation(programID, var);
	glUniform4fv(uniTrans, 1, &color[0]);
	CheckError();
}
void SetUniform(GLuint programID, glm::vec3 color, const char* var)
{
	// var - name of variable to be set in shader
	GLint uniTrans = glGetUniformLocation(programID, var);
	glUniform3fv(uniTrans, 1, &color[0]);
	CheckError();
}
void SetUniform(GLuint programID, glm::vec2 color, const char* var)
{
	// var - name of variable to be set in shader
	GLint uniTrans = glGetUniformLocation(programID, var);
	glUniform2fv(uniTrans, 1, &color[0]);
	CheckError();
}


glm::vec3 Rotate(glm::vec3 point, glm::vec4 degree)
{	
	glm::mat4 m(1);
	glm::vec4 p = glm::vec4(point.x, point.y, point.z, 0);
	p = p * glm::rotate(m, float(glm::radians(degree.x)), glm::vec3{ 0,0,1 });
	p = p * glm::rotate(m, float(glm::radians(degree.y)), glm::vec3{ 0,1,0 });
	p = p * glm::rotate(m, float(glm::radians(degree.z)), glm::vec3{ 0,0,1 });
	return degree;
}
void Rotate(GLuint programID, glm::mat4& mat, float degree, glm::vec3 axis)
{
	mat = glm::rotate(mat, glm::radians(degree), axis); // rotate 
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}
void Scale(GLuint programID, glm::mat4& mat, glm::vec3 scale)
{
	mat = glm::scale(mat, scale);
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}
void Move(GLuint programID, glm::mat4& mat, glm::vec3 scale)
{
	mat = glm::translate(mat, scale);
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}

void drawLINEs(std::vector<glm::vec3> points)
{	
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, points.capacity() * sizeof(glm::vec3), &points[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(
		0,							// axes shader variables at location 0
		3,							// number of atrributes
		GL_FLOAT,					// type
		GL_FALSE,					// normalized?
		sizeof(glm::vec3),               // stride: total size of 1 triangle
		(void*)0					// size of offset from start
	);
	glDrawArrays(GL_LINES, 0, points.size());
}

void drawLINE(glm::vec3 a, glm::vec3 b, glm::vec4 color)
{
	struct data {
		glm::vec3 point;
		glm::vec4 color;
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




};

