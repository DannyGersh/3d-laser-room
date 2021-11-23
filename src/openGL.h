#include "pch.h"
#include "depend/debug.h"


namespace shad
{
	
GLuint compile(std::wstring file_path, int shaderTYPE)
{
	// shaderTYPE:  GL_VERTEX_SHADER , GL_FRAGMENT_SHADER

	GLuint ShaderID = glCreateShader(shaderTYPE);
	std::string data;
	std::ifstream file(file_path.c_str(), std::ios::in | std::ios::ate);
	
	if( !file ) { debug::raise({ std::wstring(L"File not found: ") + file_path, {DBINFO}, debug::warning }); };
	
	if( file.is_open() )
	{
		int fileSize = file.tellg();
		data.resize(fileSize);
		file.seekg(0, file.beg);
		
		for( int i=0 ; i<fileSize ; i++ ) data[i] = file.get();
		file.close();
	}
	
	const char* raw = data.c_str();
	glShaderSource(ShaderID, 1, &raw, NULL);
	glCompileShader(ShaderID);
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
