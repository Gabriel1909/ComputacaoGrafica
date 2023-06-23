#include "Mesh.h"

void Mesh::initialize(GLuint VAO, int nVertices, Shader* shader, GLuint texID, Material material) {
	this->VAO = VAO;
	this->nVertices = nVertices;
	this->shader = shader;
	this->texID = texID;
	this->material = material;
}

void Mesh::update() {
	
}

void Mesh::draw() {
	(*shader).setFloat("ka", material.ka);
	(*shader).setFloat("kd", material.kd);
	(*shader).setFloat("ks", material.ks);
	(*shader).setFloat("q", material.d);

	glBindTexture(GL_TEXTURE_2D, texID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, nVertices);
	glBindVertexArray(0);
}
