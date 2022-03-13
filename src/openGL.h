#include <GL/glew.h>
#include "depend/debug.h"
typedef std::wstring WS; 

void GLerror(debug::Info i)
{
	GLenum err = glGetError();

	switch (err)
	{

	case GL_NO_ERROR:
		break;

	case GL_INVALID_ENUM:
		debug::db({ L"openGL error: GLenum argument out of range", i, debug::warning });
	case GL_INVALID_VALUE:
		debug::db({ L"openGL error: Numeric argument out of range.", i, debug::warning });
	case GL_INVALID_OPERATION:
		debug::db({ L"openGL error: Operation illegal in current state.", i, debug::warning });
	case GL_STACK_OVERFLOW:
		debug::db({ L"openGL error: Function would cause a stack overflow.", i, debug::warning });
	case GL_STACK_UNDERFLOW:
		debug::db({ L"openGL error: Function would cause a stack underflow.", i, debug::warning });
	case GL_OUT_OF_MEMORY:
		debug::db({ L"openGL error: Not enough memory left to execute function.", i, debug::warning });
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


template<typename T> 
void updateResolution(GLuint program, T x, T y)
{
	GLint uniform = glGetUniformLocation(program, "iResolution");
	glUniform2f(uniform, x, y);
	glViewport( 0, 0, x, y );
}
	
void set_uniColor(GLuint programID, void* color)
{
	GLint uniform = glGetUniformLocation(programID, "inCOLOR");
	glUniform4fv(uniform, 1, (GLfloat*)color);
}

float dummyMat4[16];
void M3M4_toUbo(void* mat3)
{
	// this functions sends a mat3 uniform to a ubo,
	// ubo's onley except mat4 (sortoff ...)
	// so the data in the matrix needs to be adjusted.
	//
	// this complexity is redundent.
	// I did it like this for practice,
	// also it should be slightly faster than iterating 16 elements.
	
	float* pointer2 = (float*)(mat3);
	memcpy(dummyMat4, pointer2, 36); // 4*9 - 9 floats is the entire matrix being copied
	memcpy(dummyMat4+4, pointer2+3, 24); // 4*6 - 6 floats being copied
	memcpy(dummyMat4+8, pointer2+6, 12); // 3*3 - 3 floats being copied
	glBufferSubData(GL_UNIFORM_BUFFER, 4*(4+4+4), 64, dummyMat4); // 16=4*4, 64=4*16
}	

namespace draw
{
	// this functions except anny input,
	// and convert the underling memory as needed.
	// make sure the data structures you pass to this functions
	// hase the required structure to function as intended.
	
	void line(void* A, void* B)
	{
		float* a = (float*)A;
		float* b = (float*)B;
		float data[] = { a[0], a[1], a[2], b[0], b[1], b[2] };
		
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
	void lines(void* Data, GLuint buffer)
	{
		// input: std::vector of anny sort.
		// structure of the vector:
		// 3 floats per index of the vector.
		
		std::vector<float>* data = (std::vector<float>*)(Data);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(*data).size(), &(*data)[0], GL_STATIC_DRAW);
		
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
		glDrawArrays(GL_LINE_STRIP, 0, (*data).size()/3);
	}
	void triangle(void* A, void* B, void* C, GLuint buffer)
	{	
		// input: three variables A,B,C , each of wich is a structure comprised of 3 floats.

		float* a = ((float*)A);
		float* b = ((float*)B);
		float* c = ((float*)C);
		const float data[] = 
		{ 
			a[0], a[1], a[2],
			b[0], b[1], b[2],
			c[0], c[1], c[2] 
		};
		
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
	void triangles(void* Data, GLuint buffer)
	{			 
		std::vector<float>* data = (std::vector<float>*)(Data);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(*data).size(), &(*data)[0], GL_STATIC_DRAW);
		
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
		glDrawArrays(GL_TRIANGLES, 0, (*data).size());
	}
	void triangles_index(unsigned int indices, GLuint buffer, GLuint index_buffer, unsigned int vertexAttr)
	{			 
		glEnableVertexAttribArray(0);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			0,							// axes shader variables at location 0
			3,							// number of atrributes
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(float)*vertexAttr,   // stride: total size of 1 triangle
			(void*)0					// size of offset from start
		);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices, 		   // count
			GL_UNSIGNED_INT,   // type
			(void*)0           // element array buffer offset
		);
	}
}



glm::mat3 nullMat3 = glm::mat3(1);
glm::vec3 nullVec3 = glm::vec3(0,0,0);