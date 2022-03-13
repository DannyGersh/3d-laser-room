#include "openGL.h"

struct Camera
{
	glm::mat3 trans = glm::mat3(1);
	glm::mat3 rotateX = glm::mat3(1);
	glm::mat3 rotateY = glm::mat3(1);
	glm::mat3 rotateZ = glm::mat3(1);
	glm::vec3 translate = glm::vec3(0,0,0);
	glm::vec3 direction = glm::vec3(0,0,1);
	GLuint buffer;
	
	void setup(GLuint* program, unsigned int programSize, unsigned int bindingPoint)
	{
		// "program" - array of programs that will use the camera.
		// "programSize" - size of array
		// "bindingPoint" - uniform buffer binding point
		
		glGenBuffers(1, &buffer); 
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		
		GLuint blockIndex = glGetUniformBlockIndex(program[0], "GlobalUniform");
		glUniformBlockBinding(program[0], blockIndex, bindingPoint);
		glUniformBlockBinding(program[1], blockIndex, bindingPoint);
		
		glBufferData(GL_UNIFORM_BUFFER, 4*(4+4+4+16), 0, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, buffer);		
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*4, &translate[0]);
		
		M3M4_toUbo(&trans[0]);
	}
	
	void move()
	{
		translate.z -= .01;
		update();
	}
	void rotate(unsigned int axis, float rad)
	{
		float a = cos(rad);
		float b = sin(rad);
	
		switch(axis)
		{
			case 0:
			{
				rotateX[1][1] = a;
				rotateX[1][2] = -b;
				rotateX[2][1] = b;
				rotateX[2][2] = a;
				break;
			}
			case 1:
			{
				rotateY[0][0] = a;
				rotateY[0][2] = -b;
				rotateY[2][0] = b;
				rotateY[2][2] = a;
				break;
			}
			case 2:
			{
				rotateZ[0][0] = a;
				rotateZ[0][1] = -b;
				rotateZ[1][0] = b;
				rotateZ[1][1] = a;
				break;
			}
			default: 
			{
				debug::db({ L"engine error in rotate(): invalid axis parameter.", {DBINFO}, debug::warning });
			}
		}
		
		//direction = rotateX*rotateY*rotateZ * direction;
	}
	void update()
	{
		trans = rotateY * rotateX * rotateZ;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*4, &translate[0]);
		M3M4_toUbo(&trans[0]);
	}
	void updateResolution(int x, int y)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 4*(4), 4*4, &glm::vec2(x, y)[0]);
		glViewport( 0, 0, x, y );
	}
	void updateMouse(float x, float y)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, 4*(4+4), 4*4, &glm::vec2(x, y)[0]);
	}
	void use()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	}
	
} cam;

struct object
{
	glm::mat3 trans = glm::mat3(1);
	glm::mat3 scale = glm::mat3(1);
	glm::mat3 rotateX = glm::mat3(1);
	glm::mat3 rotateY = glm::mat3(1);
	glm::mat3 rotateZ = glm::mat3(1);
	glm::vec3 translate = glm::vec3(0,0,0);
	
	GLuint program;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	
	GLuint Utrans;
	GLuint UiMouse;
	GLuint UiResolution;
	GLuint Uloc;
	
	unsigned int vertexAtrr;
	int indexSize;
	int vertexSize;
	
	void shader(std::wstring vertex, std::wstring fragment)
	{
		GLuint vertexShader = shad::compile(vertex, GL_VERTEX_SHADER);
		GLuint fragmentShader = shad::compile(fragment, GL_FRAGMENT_SHADER);
		
		program = glCreateProgram();
		prog::link(program, vertexShader, fragmentShader);
		glUseProgram(program);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
		
		Utrans = glGetUniformLocation(program, "trans");
		UiMouse = glGetUniformLocation(program, "iMouse");
		UiResolution = glGetUniformLocation(program, "iResolution");
		Uloc = glGetUniformLocation(program, "loc");		

		debug::getAll(DBINFO);
	}
	void setMesh(void* vertexData, int vertexNum, unsigned int* indexData, int indexNum, unsigned int VertexAtrr)
	{
		// vertexData - pointer to first index of any tightly packed container of vertices.
		// vertexNum - number of vertices
		// indexData - pointer to first index of any tightly packed container of indices.
		// indexNum - number of indices
		// vertexAttr - number of vertex atrributes
		
		float* data = (float*)(vertexData);
		
		glUseProgram(program);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*vertexNum, indexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertexNum*VertexAtrr, data, GL_STATIC_DRAW);
		
		indexSize = indexNum;
		vertexSize = vertexNum;
		vertexAtrr = VertexAtrr;
	}
	void draw()
	{
		glUseProgram(program);
		draw::triangles_index(indexSize, vertexBuffer, indexBuffer, vertexAtrr);
	}
	void move(glm::vec3 pos)
	{
		translate = pos;
	}
	void rotate(unsigned int axis, float rad)
	{
		float a = cos(rad);
		float b = sin(rad);
	
		switch(axis)
		{
			case 0:
			{
				rotateX[1][1] = a;
				rotateX[1][2] = -b;
				rotateX[2][1] = b;
				rotateX[2][2] = a;
				break;
			}
			case 1:
			{
				rotateY[0][0] = a;
				rotateY[0][2] = -b;
				rotateY[2][0] = b;
				rotateY[2][2] = a;
				break;
			}
			case 2:
			{
				rotateZ[0][0] = a;
				rotateZ[0][1] = -b;
				rotateZ[1][0] = b;
				rotateZ[1][1] = a;
				break;
			}
			default: 
			{
				debug::db({ L"engine error in rotate(): invalid axis parameter.", {DBINFO}, debug::warning });
			}
		}
	}
	void update()
	{
		glUseProgram(program);
		trans = rotateX * rotateY * rotateZ * scale;
		glUniformMatrix3fv(Utrans, 1, GL_TRUE, &trans[0][0]);
		glUniform3fv(Uloc, 1, &translate.x);
	}

};