#pragma once

#include <stb_image.h>

#include <vector>

#include "Mesh.h"
#include "Material.h"
#include <unordered_map>

using namespace std;

class Object {
public:
	Object() {}
	void initialize(string filePath, Shader* shader, glm::vec3 position = glm::vec3(0.0f), glm::vec3 scale = glm::vec3(1), float angle = 0.0, glm::vec3 axis = glm::vec3(0.0, 0.0, 1.0));
	void update();
	void draw();
public:
	void loadObj(string filePath);
	void loadMTL(string filePath);
	int generateTexture(string filePath);
	GLuint generateVAO(vector <GLfloat> vertbuffer, int& nVertices);
	vector <Mesh> grupos;
	glm::vec3 position;
	float angle;
	glm::vec3 axis;
	glm::vec3 scale;
	Shader* shader;
	unordered_map<std::string, Material> materiais;
};