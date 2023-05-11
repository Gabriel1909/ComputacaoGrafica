#include <iostream>
#include <string>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Mesh.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 v_color;
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int loadSimpleOjb(string filePath, int& nVertices, glm::vec3 color);

const GLuint WIDTH = 1000, HEIGHT = 1000;

char rotateChar;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime, lastFrame, lastX, lastY, yaw = -90, pitch;
float fov = 45.0f;
bool firstMouse = true;

vector <Vertex> vertices;
vector <GLuint> indices;
vector <glm::vec3> normals;
vector <glm::vec2> texCoords;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho Grau A", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("Hello3D.vs", "Hello3D.fs");
	shader.use();

	int nVertices;
	GLuint VAO = loadSimpleOjb("../../Common/3D_Models/pikachu.obj", nVertices, glm::vec3(1.0f, 0.0f, 0.0f));
	GLuint VAO2 = loadSimpleOjb("../../Common/3D_Models/pikachu.obj", nVertices, glm::vec3(0.0f, 1.0f, 0.0f));
	GLuint VAO3 = loadSimpleOjb("../../Common/3D_Models/pikachu.obj", nVertices, glm::vec3(0.0f, 0.0f, 1.0f));

	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");

	shader.setFloat("ka", 0.4);
	shader.setFloat("kd", 0.5);
	shader.setFloat("ks", 0.5);
	shader.setFloat("q", 10);

	shader.setVec3("lightPos", 5.0f, 15.0f, 5.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	Mesh pikachu1, pikachu2, pikachu3;
	pikachu1.initialize(VAO, nVertices, &shader, glm::vec3(-3.0, 0.0, 0.0));
	pikachu2.initialize(VAO2, nVertices, &shader);
	pikachu3.initialize(VAO3, nVertices, &shader, glm::vec3(3.0, 0.0, 0.0));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat) glfwGetTime();

		glm::mat4 model = glm::mat4(1);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 view = glm::lookAt(cameraPos,
			cameraPos + cameraFront,
			cameraUp);

		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		glm::mat4 projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		switch (rotateChar) {
			case 'X':
				pikachu1.
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case 'Y':
				model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
				break;

			case 'Z':
				model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
				break;

			case '1':
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case '2':
				model = glm::rotate(model, glm::radians(90.0f) * 2, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case '3':
				model = glm::rotate(model, glm::radians(90.0f) * 3, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case '4':
				model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				break;

			case '5':
				model = glm::rotate(model, glm::radians(90.0f) * 3, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
		}

		//GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		pikachu1.update();
		pikachu1.draw();

		pikachu2.update();
		pikachu2.draw();

		pikachu3.update();
		pikachu3.draw();

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	
	float cameraSpeed = 50.0f * deltaTime;

	switch (key) {

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_X:
			rotateChar = 'X';
			break;

		case GLFW_KEY_Y:
			rotateChar = 'Y';
			break;

		case GLFW_KEY_Z:
			rotateChar = 'Z';
			break;

		case GLFW_KEY_W:
			cameraPos += cameraSpeed * cameraFront;
			break;

		case GLFW_KEY_S:
			cameraPos -= cameraSpeed * cameraFront;
			break;

		case GLFW_KEY_A:
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;

		case GLFW_KEY_D:
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;

		case GLFW_KEY_1:
			rotateChar = '1';
			break;

		case GLFW_KEY_2:
			rotateChar = '2';
			break;

		case GLFW_KEY_3:
			rotateChar = '3';
			break;

		case GLFW_KEY_4:
			rotateChar = '4';
			break;

		case GLFW_KEY_5:
			rotateChar = '5';
			break;

		default:
			rotateChar = NULL;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;

	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset*0.1;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

int loadSimpleOjb(string filePath, int& nVertices, glm::vec3 color) {

	ifstream inputFile;
	inputFile.open(filePath);

	if (!inputFile.is_open()) {
		cout << "Erro ao abrir arquivo no caminho " << filePath << endl;
		return -1;
	}

	char line[100];
	string sLine;
	vector<GLfloat> vertBuffer;

	while (!inputFile.eof()) {
		inputFile.getline(line, 100);
		sLine = line;

		string word;

		istringstream ssLine(line);

		ssLine >> word;

		if (word == "v") {
			Vertex v;
			ssLine >> v.position.x >> v.position.y >> v.position.z;
			v.v_color.r = color.r; 
			v.v_color.g = color.g; 
			v.v_color.b = color.b;

			vertices.push_back(v);

		} else if (word == "vn") {
			glm::vec3 vn;
			ssLine >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		else if (word == "vt") {
			glm::vec2 vt;
			ssLine >> vt.s >> vt.t;
			texCoords.push_back(vt);

		} else if (word == "f") {
			string tokens[3];

			for (int i = 0; i < 3; i++) {
				ssLine >> tokens[i];
				int pos = tokens[i].find("/");
				string token = tokens[i].substr(0, pos);
				int index = atoi(token.c_str()) - 1;
				indices.push_back(index);
				vertBuffer.push_back(vertices[index].position.x);
				vertBuffer.push_back(vertices[index].position.y);
				vertBuffer.push_back(vertices[index].position.z);
				vertBuffer.push_back(vertices[index].v_color.r);
				vertBuffer.push_back(vertices[index].v_color.g);
				vertBuffer.push_back(vertices[index].v_color.b);

				tokens[i] = tokens[i].substr(pos + 1);
				pos = tokens[i].find("/");
				token = tokens[i].substr(0, pos);
				int indexT = atoi(token.c_str()) - 1;
				vertBuffer.push_back(texCoords[indexT].s);
				vertBuffer.push_back(texCoords[indexT].t);

				tokens[i] = tokens[i].substr(pos + 1);
				token = tokens[i].substr(0, pos);
				int indexN = atoi(token.c_str()) - 1;
				vertBuffer.push_back(normals[indexN].x);
				vertBuffer.push_back(normals[indexN].y);
				vertBuffer.push_back(normals[indexN].z);
			}
		}
	}

	inputFile.close();

	nVertices = vertBuffer.size() / 11;

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertBuffer.size() * sizeof(GLfloat), vertBuffer.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	//Posicao: X, Y, Z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Cor: R, G, B
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Coordenadas de textura: S, T
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Vetor normal: X, Y, Z
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}
