#include "Object.h"

void Object::initialize(string filePath, Shader* shader, glm::vec3 position, glm::vec3 scale, float angle, glm::vec3 axis) {
	this->position = position;
	this->scale = scale;
	this->angle = angle;
	this->axis = axis;
	this->shader = shader;

	loadObj(filePath);
}

void Object::update() {
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, position);
	model = glm::rotate(model, glm::radians(angle), axis);
	model = glm::scale(model, scale);
	shader->setMat4("model", glm::value_ptr(model));
}

void Object::draw() {
	for (int i = 0; i < grupos.size(); i++)
	{
		grupos[i].draw();
	}
}

void Object::loadObj(string filePath) {
	string texNames[] = { "../../Common/3D_Models/Pokemon/textures/PikachuMouthDh.png",
		"../../Common/3D_Models/Pokemon/textures/PikachuDh.png",
		"../../Common/3D_Models/Pokemon/texturesPikachuHohoDh.png",
		"../../Common/3D_Models/Pokemon/texturesPikachuEyeDh.png",
		"../../Common/3D_Models/Pokemon/texturesPikachuDh.png" };

	int iTexture = 0;

	ifstream inputFile;
	inputFile.open(filePath);
	vector <GLfloat> vertbuffer;

	vector <glm::vec3> vertices, colors;
	vector <glm::vec3> normals;
	vector <glm::vec2> texCoord;

	bool initializeGroup = true;

	if (!inputFile.is_open()) {
		cout << "Erro ao abrir arquivo no caminho " << filePath << endl;
		return;
	}

	char line[100];
	string sline;


	while (!inputFile.eof()) {
		inputFile.getline(line, 100);
		sline = line;

		string word;
		istringstream ssline(sline);

		ssline >> word;

		if (initializeGroup) {

		}

		if (word == "v" || inputFile.eof()) {

			if (initializeGroup) {


				if (vertbuffer.size()) 	{
					initializeGroup = false;
					Mesh m;
					int nVertices;
					GLuint VAO = generateVAO(vertbuffer, nVertices);
					GLuint texID = generateTexture(texNames[iTexture]);
					iTexture++;
					m.initialize(VAO, nVertices, shader, texID);
					grupos.push_back(m);
					vertbuffer.clear();
				}
			}

			glm::vec3 v, color;
			ssline >> v.x >> v.y >> v.z;
			color.r = 1.0; color.g = 0.0; color.b = 0.0;
			vertices.push_back(v);
			colors.push_back(color);
		}
		if (word == "vt") {
			glm::vec2 vt;
			ssline >> vt.s >> vt.t;
			texCoord.push_back(vt);
		}
		if (word == "vn") {
			glm::vec3 vn;
			ssline >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}

		if (word == "g") {
			initializeGroup = true;
		}
		if (word == "f") {
			string tokens[3];
			for (int i = 0; i < 3; i++) {
				ssline >> tokens[i];
				int pos = tokens[i].find("/");
				string token = tokens[i].substr(0, pos);
				int index = atoi(token.c_str()) - 1;

				vertbuffer.push_back(vertices[index].x);
				vertbuffer.push_back(vertices[index].y);
				vertbuffer.push_back(vertices[index].z);
				vertbuffer.push_back(colors[index].r);
				vertbuffer.push_back(colors[index].g);
				vertbuffer.push_back(colors[index].b);

				tokens[i] = tokens[i].substr(pos + 1);
				pos = tokens[i].find("/");
				token = tokens[i].substr(0, pos);
				int indexT = atoi(token.c_str()) - 1;

				vertbuffer.push_back(texCoord[indexT].s);
				vertbuffer.push_back(texCoord[indexT].t);

				tokens[i] = tokens[i].substr(pos + 1);
				token = tokens[i].substr(0, pos);
				int indexN = atoi(token.c_str()) - 1;

				vertbuffer.push_back(normals[indexN].x);
				vertbuffer.push_back(normals[indexN].y);
				vertbuffer.push_back(normals[indexN].z);
			}
		}
	}

	inputFile.close();
}

GLuint Object::generateVAO(vector<GLfloat> vertbuffer, int& nVertices) {
	nVertices = vertbuffer.size() / 11;

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertbuffer.size() * sizeof(GLfloat), vertbuffer.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenadas de textura (s, t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Atributo vetor normal (x, y e z)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

int Object::generateTexture(string filePath) {
	GLuint texID;

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (data) {
		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
				data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
				data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}