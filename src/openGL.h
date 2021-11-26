#include "pch.h"
#include "depend/debug.h"


void GLerror()
{
	GLenum err = glGetError();

	switch (err)
	{

	case GL_NO_ERROR:
		break;

	case GL_INVALID_ENUM:
		debug::db({ L"openGL error: GLenum argument out of range\n", {DBINFO}, debug::warning });
	case GL_INVALID_VALUE:
		debug::db({ L"openGL error: Numeric argument out of range.\n", {DBINFO}, debug::warning });
	case GL_INVALID_OPERATION:
		debug::db({ L"openGL error: Operation illegal in current state.\n", {DBINFO}, debug::warning });
	case GL_STACK_OVERFLOW:
		debug::db({ L"openGL error: Function would cause a stack overflow.\n", {DBINFO}, debug::warning });
	case GL_STACK_UNDERFLOW:
		debug::db({ L"openGL error: Function would cause a stack underflow.\n", {DBINFO}, debug::warning });
	case GL_OUT_OF_MEMORY:
		debug::db({ L"openGL error: Not enough memory left to execute function.\n", {DBINFO}, debug::warning });
	}

}


namespace shad
{
	
std::string readFile(std::wstring file_path)
{
	std::string data;
	std::ifstream file(file_path.c_str(), std::ios::in | std::ios::ate);
	
	if( !file ) debug::raise({ std::wstring(L"File not found: ") + file_path, {DBINFO}, debug::warning });
	if( file.is_open() )
	{
		int fileSize = file.tellg();
		data.resize(fileSize);
		file.seekg(0, file.beg);
		
		for( int i=0 ; i<fileSize ; i++ ) data[i] = file.get();
		file.close();
	}
	
	return data;
}

GLuint compile(const char* string, GLuint type)
{
	// type:  GL_VERTEX_SHADER , GL_FRAGMENT_SHADER
	
	GLuint ShaderID = glCreateShader(type);
	glShaderSource(ShaderID, 1, &string, NULL);
	glCompileShader(ShaderID);
	
	GLint compile_status = 0;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compile_status);

	if(compile_status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &maxLength);
				
		GLchar* infoLog = new GLchar[maxLength];
		glGetShaderInfoLog(ShaderID, maxLength, &maxLength, infoLog);
		
		glDeleteShader(ShaderID);
		
		//static_cast<const char*>(infoLog);
		debug::raise({ std::wstring(L"openGL error; Failed to compile shader:\n") , {DBINFO}, debug::warning });
	}
	
	return ShaderID;
}


}


namespace prog
{
	
void link(GLuint programID, GLuint vectSHADER, GLuint fragSHADER)
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

}

void UniformData(GLuint programID, glm::mat4 mat)
{
	GLint uniTrans = glGetUniformLocation(programID, "trans"); // get the 'trans' uniform from shader
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(mat)); // update the uniform
}
void set_uniCOLOR(GLuint _uniCOLOR_programID, glm::vec4 color)
{
	GLint uniTrans = glGetUniformLocation(_uniCOLOR_programID, "inCOLOR");
	glUniform4fv(uniTrans, 1, &color[0]);
}