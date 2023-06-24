#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Material.h"

class Mesh {
public:
	Mesh(GLuint VAO, int nVertices, Shader* shader, GLuint texID, Material material) {
		this->VAO = VAO;
		this->nVertices = nVertices;
		this->shader = shader;
		this->texID = texID;
		this->material = material;
	}
	
	void draw();

protected:
	GLuint VAO;
	int nVertices;
	Shader* shader;
	GLuint texID;
	Material material;
};
