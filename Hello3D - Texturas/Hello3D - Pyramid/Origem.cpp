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
#include "Hermite.h"
#include "Bezier.h"
#include "CatmullRom.h"
#include "Curve.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void operacoesTeclado();
void iniciarParametros(Shader* shader, int width, int height, GLint viewLoc);
void iniciarObjetos(Shader* shader, Json::Value json);
vector<glm::vec3> generateUnisinosPointsSet(); 
void iniciarCameraELuz(Shader* shader, int width, int height, Json::Value json);

const GLuint WIDTH = 1000, HEIGHT = 1000;

glm::vec3 cameraPos;
glm::vec3 cameraFront;
glm::vec3 cameraUp;
float cameraSpeed;
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

	GLint viewLoc = glGetUniformLocation(shader.ID, "view");

	iniciarParametros(&shader, width, height, viewLoc);

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

void iniciarParametros(Shader* shader, int width, int height, GLint viewLoc) {

	ifstream inputFile("./parametros.json");

	if (!inputFile.is_open()) {
		cout << "Não foi possivel abrir o arquivo parametros.json" << endl;
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
		cout << "Não foi possivel fazer o parse do JSON: " << parseErrors << endl;
		return;
	}

	iniciarObjetos(shader, json);
	iniciarCameraELuz(shader, width, height, json);
}

void iniciarObjetos(Shader* shader, Json::Value json) {

	for (const auto& objetoJson : json["objetos"]) {
		string arquivo = objetoJson["arquivo"].asString();
		Json::Value transformacao = objetoJson["transformacaoInicial"];
		Json::Value posicao = transformacao["posicao"];
		Json::Value escala = transformacao["escala"];
		Json::Value rotacao = transformacao["rotacao"];
		float anguloRotacao = transformacao["anguloRotacao"].asFloat();

		Object obj(arquivo, shader, glm::vec3(posicao[0].asFloat(), posicao[1].asFloat(), posicao[2].asFloat()),
			glm::vec3(escala[0].asFloat(), escala[1].asFloat(), escala[2].asFloat()),
			anguloRotacao, glm::vec3(rotacao[0].asFloat(), rotacao[1].asFloat(), rotacao[2].asFloat()));

		Json::Value animacao = objetoJson["animacao"];

		if (!animacao.isNull()) {
			Bezier bezier;

			bezier.setControlPoints(generateUnisinosPointsSet());
			bezier.setShader(shader);
			bezier.generateCurve(500);

			obj.curve = bezier;
		}

		objetos.push_back(obj);
	}
}

vector<glm::vec3> generateUnisinosPointsSet() {

	float vertices[] = {
			-0.262530, 0.376992, 0.000000,
			-0.262530, 0.377406, 0.000000,
			-0.262530, 0.334639, 0.000000,
			-0.262530, 0.223162, 0.000000,
			-0.262530, 0.091495, 0.000000,
			-0.262371, -0.006710, 0.000000,
			-0.261258, -0.071544, -0.000000,
			-0.258238, -0.115777, -0.000000,
			-0.252355, -0.149133, -0.000000,
			-0.242529, -0.179247, -0.000000,
			-0.227170, -0.208406, -0.000000,
			-0.205134, -0.237216, -0.000000,
			-0.177564, -0.264881, -0.000000,
			-0.146433, -0.289891, -0.000000,
			-0.114730, -0.309272, -0.000000,
			-0.084934, -0.320990, -0.000000,
			-0.056475, -0.328224, -0.000000,
			-0.028237, -0.334170, -0.000000,
			0.000000, -0.336873, -0.000000,
			0.028237, -0.334170, -0.000000,
			0.056475, -0.328224, -0.000000,
			0.084934, -0.320990, -0.000000,
			0.114730, -0.309272, -0.000000,
			0.146433, -0.289891, -0.000000,
			0.177564, -0.264881, -0.000000,
			0.205134, -0.237216, -0.000000,
			0.227170, -0.208406, -0.000000,
			0.242529, -0.179247, -0.000000,
			0.252355, -0.149133, -0.000000,
			0.258238, -0.115777, -0.000000,
			0.261258, -0.071544, -0.000000,
			0.262371, -0.009704, 0.000000,
			0.262530, 0.067542, 0.000000,
			0.262769, 0.153238, 0.000000,
			0.264438, 0.230348, 0.000000,
			0.268678, 0.284286, 0.000000,
			0.275462, 0.320338, 0.000000,
			0.284631, 0.347804, 0.000000,
			0.296661, 0.372170, 0.000000,
			0.311832, 0.396628, 0.000000,
			0.328990, 0.419020, 0.000000,
			0.347274, 0.436734, 0.000000,
			0.368420, 0.450713, 0.000000,
			0.393395, 0.462743, 0.000000,
			0.417496, 0.474456, 0.000000,
			0.436138, 0.487056, 0.000000,
			0.450885, 0.500213, 0.000000,
			0.464572, 0.513277, 0.000000,
			0.478974, 0.525864, 0.000000,
			0.494860, 0.538133, 0.000000,
			0.510031, 0.552151, 0.000000,
			0.522127, 0.570143, 0.000000,
			0.531124, 0.593065, 0.000000,
			0.537629, 0.620809, 0.000000,
			0.542465, 0.650303, 0.000000,
			0.546798, 0.678259, 0.000000,
			0.552959, 0.703513, 0.000000,
			0.563121, 0.725745, 0.000000,
			0.577656, 0.745911, 0.000000,
			0.596563, 0.764858, 0.000000,
			0.620160, 0.781738, 0.000000,
			0.648302, 0.795385, 0.000000,
			0.678670, 0.805057, 0.000000,
			0.710336, 0.810741, 0.000000,
			0.750111, 0.814914, 0.000000,
			0.802994, 0.819945, 0.000000,
			0.860771, 0.825435, 0.000000,
	};

	vector <glm::vec3> uniPoints;

	for (int i = 0; i < 67 * 3; i += 3)	{
		glm::vec3 point;
		point.x = vertices[i];
		point.y = vertices[i + 1];
		point.z = 0.0;

		uniPoints.push_back(point);
	}

	return uniPoints;
}

void iniciarCameraELuz(Shader* shader, int width, int height, Json::Value json) {

	Json::Value posicaoLuz = json["iluminacao"]["posicao"];
	Json::Value corLuz = json["iluminacao"]["cor"];

	(*shader).setVec3("lightPos", posicaoLuz[0].asFloat(), posicaoLuz[1].asFloat(), posicaoLuz[2].asFloat());
	(*shader).setVec3("lightColor", corLuz[0].asFloat(), corLuz[1].asFloat(), corLuz[2].asFloat());

	glm::mat4 model = glm::mat4(1);
	GLint modelLoc = glGetUniformLocation((*shader).ID, "model");

	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	Json::Value camera = json["camera"];

	Json::Value posicaoCamera = camera["posicao"];
	Json::Value frontCamera = camera["front"];
	Json::Value upCamera = camera["up"];

	cameraPos = glm::vec3(posicaoCamera[0].asFloat(), posicaoCamera[1].asFloat(), posicaoCamera[2].asFloat());
	cameraFront = glm::vec3(frontCamera[0].asFloat(), frontCamera[1].asFloat(), frontCamera[2].asFloat());
	cameraUp = glm::vec3(upCamera[0].asFloat(), upCamera[1].asFloat(), upCamera[2].asFloat());
	cameraSpeed = camera["velocidade"].asFloat();

	Json::Value frustum = camera["frustum"];

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, frustum["proximidade"].asFloat(), frustum["distancia"].asFloat());
	GLint projLoc = glGetUniformLocation((*shader).ID, "projection");

	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));
}