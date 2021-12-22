#include "pch.h"
#include "depend/debug.h"
typedef std::wstring WS; 

void GLerror()
{
	GLenum err = glGetError();

	switch (err)
	{

	case GL_NO_ERROR:
		break;

	case GL_INVALID_ENUM:
		debug::db({ L"openGL error: GLenum argument out of range", {DBINFO}, debug::warning });
	case GL_INVALID_VALUE:
		debug::db({ L"openGL error: Numeric argument out of range.", {DBINFO}, debug::warning });
	case GL_INVALID_OPERATION:
		debug::db({ L"openGL error: Operation illegal in current state.", {DBINFO}, debug::warning });
	case GL_STACK_OVERFLOW:
		debug::db({ L"openGL error: Function would cause a stack overflow.", {DBINFO}, debug::warning });
	case GL_STACK_UNDERFLOW:
		debug::db({ L"openGL error: Function would cause a stack underflow.", {DBINFO}, debug::warning });
	case GL_OUT_OF_MEMORY:
		debug::db({ L"openGL error: Not enough memory left to execute function.", {DBINFO}, debug::warning });
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
GLuint compileShader(const char* string, GLuint type, std::wstring file_path)
{
	// type:  GL_VERTEX_SHADER , GL_FRAGMENT_SHADER
	GLuint ShaderID = glCreateShader(type);
	glShaderSource(ShaderID, 1, &string, NULL);
	glCompileShader(ShaderID);
	return ShaderID;
}
GLuint compile(std::wstring file_path, GLuint type)
{
	// type:  GL_VERTEX_SHADER , GL_FRAGMENT_SHADER
	
	GLuint ShaderID = compileShader(readFile(file_path).c_str(), type, file_path);
	
	GLint compile_status = 0;
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &compile_status);

	if(compile_status == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &maxLength);
				
		GLchar* infoLog = new GLchar[maxLength];
		glGetShaderInfoLog(ShaderID, maxLength, &maxLength, infoLog);
		
		glDeleteShader(ShaderID);
		
		const char* glErr = static_cast<const char*>(infoLog);
		debug::raise({ WS(L"openGL error; Failed to compile shader: ")+WS(file_path+L"\n"+glErr) , {DBINFO}, debug::warning });
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
	GLint uniform = glGetUniformLocation(programID, "trans");
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mat));
}
void set_uniColor(GLuint programID, glm::vec4 color)
{
	GLint uniform = glGetUniformLocation(programID, "inCOLOR");
	glUniform4fv(uniform, 1, &color[0]);
}


namespace draw
{
	void line(glm::vec3 a, glm::vec3 b)
	{
		const glm::vec3 data[] = { a, b };
	
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(3+3), data, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			0,							// axes shader variables at location 0
			3,							// number of atrributes
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(float)*3,            // stride: total size of 1 triangle
			(void*)0					// size of offset from start
		);
		glDrawArrays(GL_LINES, 0, 3);
		
		glDeleteBuffers(1, &buffer);
	}
	void line(glm::vec3 a, glm::vec3 b, GLuint buffer)
	{
		const glm::vec3 data[] = { a, b };

		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(3+3), data, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			0,							// axes shader variables at location 0
			3,							// number of atrributes
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(float)*3,            // stride: total size of 1 triangle
			(void*)0					// size of offset from start
		);
		glDrawArrays(GL_LINES, 0, 3);
	}
	void lines(std::vector<glm::vec3>& data, GLuint buffer)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*data.size(), &data[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			0,							// axes shader variables at location 0
			3,							// number of atrributes
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(float)*3,            // stride: total size of 1 triangle
			(void*)0					// size of offset from start
		);
		glDrawArrays(GL_LINE_STRIP, 0, data.size());
	}
	void triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, GLuint buffer)
	{
		const glm::vec3 data[] = { a, b, c };
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*3, data, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			0,							// axes shader variables at location 0
			3,							// number of atrributes
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(float)*3,            // stride: total size of 1 triangle
			(void*)0					// size of offset from start
		);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}


	//void test(std::shared_ptr<void> a)
	//{
	//	auto poop = std::static_pointer_cast<char*>(a);
	//	std::cout<<(*poop.get())[0]<<(*poop.get())[1]<<(*poop.get())[2]<<(*poop.get())[3]<<'\n';
	//}
	//
	//int main()
	//{
	//	char poop[] = {'a','b','c','d'};
	//	test(std::make_shared<char*>(poop));
	//}
	void testline(std::shared_ptr<void> A, std::shared_ptr<void> B)
	{
		auto a = std::static_pointer_cast<glm::vec3>(A);
		//const glm::vec3 data[] = { a, b };
		//
		//GLuint buffer;
		//glGenBuffers(1, &buffer);
		//glBindBuffer(GL_ARRAY_BUFFER, buffer);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(3+3), data, GL_STATIC_DRAW);
		//
		//glEnableVertexAttribArray(0);
		//
		//glBindBuffer(GL_ARRAY_BUFFER, buffer);
		//glVertexAttribPointer(
		//	0,							// axes shader variables at location 0
		//	3,							// number of atrributes
		//	GL_FLOAT,					// type
		//	GL_FALSE,					// normalized?
		//	sizeof(float)*3,            // stride: total size of 1 triangle
		//	(void*)0					// size of offset from start
		//);
		//glDrawArrays(GL_LINES, 0, 3);
		//
		//glDeleteBuffers(1, &buffer);
	}
}