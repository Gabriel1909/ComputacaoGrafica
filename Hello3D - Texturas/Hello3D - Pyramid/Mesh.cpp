#include "Mesh.h"

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
