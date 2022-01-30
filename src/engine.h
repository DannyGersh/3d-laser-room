#include "openGL.h"

struct object
{
	glm::mat3 trans;
	glm::mat3 scale;
	glm::mat3 rotateX;
	glm::mat3 rotateY;
	glm::mat3 rotateZ;
	
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	unsigned int vertexAtrr;
	int indexSize;
	int vertexSize;
	
	object()
	{
		//GLerror();
		trans = glm::mat3(1);
		scale = glm::mat3(1);
		rotateX = glm::mat3(1);
		rotateY = glm::mat3(1);
		rotateZ = glm::mat3(1);
	}
	
	void shader(std::wstring vertex, std::wstring fragment)
	{
		vertexShader = shad::compile(vertex, GL_VERTEX_SHADER);
		fragmentShader = shad::compile(fragment, GL_FRAGMENT_SHADER);
		
		program = glCreateProgram();
		prog::link(program, vertexShader, fragmentShader);
		glUseProgram(program);
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &indexBuffer);
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
		trans = rotateX * rotateY * rotateZ * scale;
		UniformData(program, &trans);
	}
};