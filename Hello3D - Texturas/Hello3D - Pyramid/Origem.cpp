#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <json/json.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "Object.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void operacoesTeclado();
void iniciarObjetos(Shader* shader);

const GLuint WIDTH = 1000, HEIGHT = 1000;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraSpeed = 0.05;
char rotateChar;
float deltaTime, lastFrame;

bool firstMouse = true;
float lastX = 0.0, lastY = 0.0;
float yaw = -90.0, pitch = 0.0;
vector <Object> objetos;
int objetoSelecionado = 0;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho Grau B", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("Hello3D.vs", "Hello3D.fs");
	shader.use();

	glm::mat4 model = glm::mat4(1);
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");

	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	shader.setVec3("lightPos", -2.0f, 10.0f, 3.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	iniciarObjetos(&shader);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.ID, "colorBuffer"), 0);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);
		
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		operacoesTeclado();

		for (int i = 0; i < objetos.size(); i++) {
			objetos[i].update();
			objetos[i].draw();
		}


		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	float cameraSpeed = 50.0f * deltaTime;

	if (action == GLFW_PRESS) {

		switch (key) {

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;

			case GLFW_KEY_LEFT:
				if (objetoSelecionado == 0) {
					objetoSelecionado = objetos.size() - 1;
				}
				else {
					objetoSelecionado--;
				}
				break;

			case GLFW_KEY_RIGHT:
				if (objetoSelecionado == objetos.size() - 1) {
					objetoSelecionado = 0;
				}
				else {
					objetoSelecionado++;
				}
				break;

			case GLFW_KEY_SPACE:
				rotateChar = NULL;
				break;
			}
	}

	else {
		switch (key) {
			case GLFW_KEY_X:
				rotateChar = 'X';
				break;

			case GLFW_KEY_Y:
				rotateChar = 'Y';
				break;

			case GLFW_KEY_Z:
				rotateChar = 'Z';
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

			case GLFW_KEY_6:
				rotateChar = '6';
				break;

			case GLFW_KEY_EQUAL:
				rotateChar = '+';
				break;

			case GLFW_KEY_MINUS:
				rotateChar = '-';
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
			}
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float offsetx = xpos - lastX;
	float offsety = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	offsetx *= cameraSpeed;
	offsety *= cameraSpeed;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void operacoesTeclado() {

	float angle = (GLfloat)glfwGetTime() * 20;

	switch (rotateChar) {
		case 'X':
			objetos[objetoSelecionado].angle = angle;
			objetos[objetoSelecionado].axis = glm::vec3(1.0f, 0.0f, 0.0f);
			break;

		case 'Y':
			objetos[objetoSelecionado].angle = angle;
			objetos[objetoSelecionado].axis = glm::vec3(0.0f, 1.0f, 0.0f);
			break;

		case 'Z':
			objetos[objetoSelecionado].angle = angle;
			objetos[objetoSelecionado].axis = glm::vec3(0.0f, 0.0f, 1.0f);
			break;

		case '1':
			objetos[objetoSelecionado].position.x = objetos[objetoSelecionado].position.x - 0.001f;
			break;

		case '2':
			objetos[objetoSelecionado].position.x = objetos[objetoSelecionado].position.x + 0.001f;
			break;

		case '3':
			objetos[objetoSelecionado].position.y = objetos[objetoSelecionado].position.y - 0.001f;
			break;

		case '4':
			objetos[objetoSelecionado].position.y = objetos[objetoSelecionado].position.y + 0.001f;
			break;

		case '5':
			objetos[objetoSelecionado].position.z = objetos[objetoSelecionado].position.z - 0.001f;
			break;

		case '6':
			objetos[objetoSelecionado].position.z = objetos[objetoSelecionado].position.z + 0.001f;
			break;

		case '-':
			objetos[objetoSelecionado].scale = objetos[objetoSelecionado].scale - 0.0001f;
			break;

		case '+':
			objetos[objetoSelecionado].scale = objetos[objetoSelecionado].scale + 0.0001f;
			break;
		}
}

void iniciarObjetos(Shader* shader) {

	ifstream inputFile("./parametros.json");

	if (!inputFile.is_open()) {
		cout << "N�o foi possivel abrir o arquivo parametros.json" << endl;
		return;
	}

	string conteudo((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
	inputFile.close();

	Json::Value json;

	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	std::string parseErrors;
	bool parsingSuccessful = reader->parse(conteudo.c_str(), conteudo.c_str() + conteudo.size(), &json, &parseErrors);

	delete reader;

	if (!parsingSuccessful) {
		cout << "N�o foi possivel fazer o parse do JSON: " << parseErrors << endl;
		return;
	}

	const Json::Value objetosJson = json["objetos"];

	for (const auto& objetoJson : objetosJson) {
		string arquivo = objetoJson["arquivo"].asString();
		Json::Value transformacao = objetoJson["transformacaoInicial"];
		Json::Value posicao = transformacao["posicao"];
		Json::Value escala = transformacao["escala"];
		Json::Value rotacao = transformacao["rotacao"];
		float anguloRotacao = transformacao["anguloRotacao"].asFloat();

		Object obj(arquivo, shader, glm::vec3(posicao[0].asFloat(), posicao[1].asFloat(), posicao[2].asFloat()),
			glm::vec3(escala[0].asFloat(), escala[1].asFloat(), escala[2].asFloat()), 
			anguloRotacao, glm::vec3(rotacao[0].asFloat(), rotacao[1].asFloat(), rotacao[2].asFloat()));

		objetos.push_back(obj);
	}
}