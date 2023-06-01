#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "Shader.h"
#include "Object.h"


struct Vertex {
	glm::vec3 position;
	glm::vec3 v_color;
};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

const GLuint WIDTH = 1000, HEIGHT = 1000;

char rotateChar;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime, lastFrame, lastX, lastY, yaw = -90, pitch;
float fov = 45.0f;
bool firstMouse = true;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Olá Visualizador 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
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

	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");

	shader.setFloat("ka", 0.4);
	shader.setFloat("kd", 0.5);
	shader.setFloat("ks", 0.5);
	shader.setFloat("q", 10);

	shader.setVec3("lightPos", -2.0f, 10.0f, 3.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	Object pikachu1;
	pikachu1.initialize("../../Common/3D_Models/Pokemon/Pikachu.obj", &shader);

	glEnable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader.ID, "colorBuffer"), 0);

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

		GLint modelLoc = glGetUniformLocation(shader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		pikachu1.update();
		pikachu1.draw();

		glfwSwapBuffers(window);
	}

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