#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int setupShader();
int setupGeometry();

const GLuint WIDTH = 1000, HEIGHT = 1000;

const GLchar* vertexShaderSource = 
	"#version 400\n"
	"layout (location = 0) in vec3 position;\n"
	"layout (location = 1) in vec3 color;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"

	"out vec4 finalColor;\n"
	"void main() {\n"
		"gl_Position = projection * view * model * vec4(position, 1.0f);\n"
		"finalColor = vec4(color, 1.0);\n"
	"}\0";

const GLchar* fragmentShaderSource = 
	"#version 400\n"
	"in vec4 finalColor;\n"
	"out vec4 color;\n"
	"void main() {\n"
		"color = finalColor;\n"
	"}\n\0";

char rotateChar;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float deltaTime, lastFrame, lastX, lastY, yaw, pitch;
float fov = 45.0f;
bool firstMouse = true;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Cubo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLuint shaderID = setupShader();

	GLuint VAO = setupGeometry();

	glUseProgram(shaderID);

	GLint viewLoc = glGetUniformLocation(shaderID, "view");
	GLint projLoc = glGetUniformLocation(shaderID, "projection");

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();
		float angulo = glm::radians(90.0f);

		glm::mat4 model = glm::mat4(1);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);

		glm::mat4 view = glm::lookAt(cameraPos,// Posição (ponto da camera) 
			cameraPos + cameraFront,// Target (ponto que a camera está olhando)
			cameraUp); // Up (vetor)

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
				model = glm::rotate(model, angulo, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case '2':
				model = glm::rotate(model, angulo * 2, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case '3':
				model = glm::rotate(model, angulo * 3, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case '4':
				model = glm::rotate(model, angulo, glm::vec3(0.0f, 1.0f, 0.0f));
				break;

			case '5':
				model = glm::rotate(model, angulo * 3, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
		}

		GLint modelLoc = glGetUniformLocation(shaderID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		glDrawArrays(GL_POINTS, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	
	float cameraSpeed = 10.0f * deltaTime;

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

int setupShader() {

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

int setupGeometry() {

	GLfloat vertices[] = {

		 //Base
		 -0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		 -0.5, -0.5, 0.5, 1.0, 0.0, 1.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 1.0,

		 -0.5, -0.5, 0.5, 1.0, 0.0, 1.0,
		  0.5, -0.5, 0.5, 1.0, 0.0, 1.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 1.0,

		 //Topo
		 -0.5, 0.5, -0.5, 0.0, 0.0, 1.0,
		 -0.5, 0.5, 0.5, 0.0, 0.0, 1.0,
		  0.5, 0.5, -0.5, 0.0, 0.0, 1.0,

		 -0.5, 0.5, 0.5, 0.0, 0.0, 1.0,
		  0.5, 0.5, 0.5, 0.0, 0.0, 1.0,
		  0.5, 0.5, -0.5, 0.0, 0.0, 1.0,

		 //Frente
		 -0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		 -0.5,  0.5,  0.5, 1.0, 1.0, 0.0,
		  0.5, -0.5, 0.5, 1.0, 1.0, 0.0,

		 -0.5,  0.5, 0.5, 1.0, 1.0, 0.0,
		  0.5,  0.5,  0.5, 1.0, 1.0, 0.0,
		  0.5, -0.5, 0.5, 1.0, 1.0, 0.0,

		  //Trás
		 -0.5, -0.5, -0.5, 1.0, 0.0, 0.0,
		 -0.5,  0.5, -0.5, 1.0, 0.0, 0.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 0.0,

		 -0.5,  0.5, -0.5, 1.0, 0.0, 0.0,
		  0.5,  0.5, -0.5, 1.0, 0.0, 0.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 0.0,

		  //Esquerda
		 -0.5, -0.5, 0.5, 0.0, 1.0, 0.0,
		 -0.5, 0.5,  0.5, 0.0, 1.0, 0.0,
		 -0.5, -0.5, -0.5, 0.0, 1.0, 0.0,

		 -0.5, 0.5, 0.5, 0.0, 1.0, 0.0,
		 -0.5, 0.5, -0.5, 0.0, 1.0, 0.0,
		 -0.5, -0.5, -0.5, 0.0, 1.0, 0.0,

		  //Direita
		  0.5, -0.5, 0.5, 0.0, 1.0, 1.0,
		  0.5, 0.5,  0.5, 0.0, 1.0, 1.0,
		  0.5, -0.5, -0.5, 0.0, 1.0, 1.0,

		  0.5, 0.5, 0.5, 0.0, 1.0, 1.0,
		  0.5, 0.5, -0.5, 0.0, 1.0, 1.0,
		  0.5, -0.5, -0.5, 0.0, 1.0, 1.0,
	};

	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
 
	glBindVertexArray(VAO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	return VAO;
}