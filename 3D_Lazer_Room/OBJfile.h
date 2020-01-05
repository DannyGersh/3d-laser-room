#pragma once
#include "OBJ_Loader.h"


struct OBJfile
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normal;
	std::vector<unsigned int> indices;

	OBJfile(std::string path)
	{
		objl::Loader file;
		file.LoadFile(path);

		for (auto i : file.LoadedMeshes)
		{
			for (auto ii : i.Vertices) {
				vertices.push_back(objlVECtoGLMvec(ii.Position));
			}
			for (auto ii : i.Indices) {
				indices.push_back(ii);
			}
		}

	}

	glm::vec2 objlVECtoGLMvec(objl::Vector2 vec)
	{
		return { vec.X,vec.Y };
	}
	glm::vec3 objlVECtoGLMvec(objl::Vector3 vec)
	{
		return { vec.X,vec.Y,vec.Z };
	}

};

void initOBJfile(OBJfile& file, glm::vec4 color)
{
	// vertex buffer
	{
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, file.vertices.size() * sizeof(glm::vec3), &file.vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glEnableVertexAttribArray(0); // position
		glVertexAttribPointer(
			0,							// variable at location 0, in shader
			3,							// number of atrributes
			GL_FLOAT,					// type
			GL_FALSE,					// normalized?
			sizeof(float) * 3,          // stride: total size of 1 vertex
			(void*)0					// size of offset from start
		);

		std::vector<glm::vec4> colors(file.vertices.size());
		std::fill(colors.begin(), colors.end(), color);

		GLuint buffer2;
		glGenBuffers(1, &buffer2);
		glBindBuffer(GL_ARRAY_BUFFER, buffer2);
		glBufferData(GL_ARRAY_BUFFER, file.indices.size() * sizeof(glm::vec4), &colors[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, buffer2);

		glEnableVertexAttribArray(1); // color
		glVertexAttribPointer(
			1,
			4,
			GL_FLOAT,
			GL_FALSE,
			sizeof(float) * 4,
			(void*)0
		);
	}

	// index buffer
	{
		GLuint indexBUFFER;
		glGenBuffers(1, &indexBUFFER);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBUFFER);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (file.indices.size() * sizeof(unsigned int)), &file.indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBUFFER);
	}

}