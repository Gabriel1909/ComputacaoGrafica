#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Shader.h"
using namespace std;

class Material {
public:
	Material() {}
	~Material() {}
	void initialize(string name);

public:
	string name;
	string texName;
	float ka;
	float ks;
	float kd;
	float d;
};
