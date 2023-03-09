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
int setupShader();
int setupGeometry();

const GLuint WIDTH = 1000, HEIGHT = 1000;

const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;\n"
"uniform mat4 model;\n"
"out vec4 finalColor;\n"
"void main()\n"
"{\n"
"gl_Position = model * vec4(position, 1.0);\n"
"finalColor = vec4(color, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 400\n"
"in vec4 finalColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = finalColor;\n"
"}\n\0";

char rotateTeste;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

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

	glm::mat4 model = glm::mat4(1);
	GLint modelLoc = glGetUniformLocation(shaderID, "model");

	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();
		float angulo = glm::radians(90.0f);

		model = glm::mat4(1); 

		switch (rotateTeste) {
			case 'X':
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
				break;

			case 'Y':
				model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
				break;

			case 'Z':
				model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
				break;

			case 'W':
				model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 1.0f));
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
				model = glm::rotate(model, angulo*3, glm::vec3(0.0f, 1.0f, 0.0f));
				break;
		}

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
		
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

	if (action == GLFW_PRESS) {
	
		switch (key) {

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;

			case GLFW_KEY_X:
				rotateTeste = 'X';
				break;

			case GLFW_KEY_Y:
				rotateTeste = 'Y';
				break;

			case GLFW_KEY_Z:
				rotateTeste = 'Z';
				break;

			case GLFW_KEY_W:
				rotateTeste = 'W';
				break;

			case GLFW_KEY_1:
				rotateTeste = '1';
				break;

			case GLFW_KEY_2:
				rotateTeste = '2';
				break;

			case GLFW_KEY_3:
				rotateTeste = '3';
				break;

			case GLFW_KEY_4:
				rotateTeste = '4';
				break;

			case GLFW_KEY_5:
				rotateTeste = '5';
				break;

			default:
				rotateTeste = NULL;
		}
	}
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