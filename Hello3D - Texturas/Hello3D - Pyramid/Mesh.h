#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Material.h"

class Mesh {
public:
	Mesh() {}
	~Mesh() {}
	void initialize(GLuint VAO, int nVertices, Shader *shader, GLuint texID, Material material);
	void update();
	void draw();

protected:
	GLuint VAO;
	int nVertices;
	Shader* shader;
	GLuint texID;
	Material material;
};
