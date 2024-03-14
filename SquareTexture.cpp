#include <GL/glew.h>
#include<GL/glut.h>
#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <soil.h>
#include <iostream>
#include "Shader.h"

int main()
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	float vertices[] = {
	-0.6f, -0.1f,  0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //1
	-0.5f, 0.1f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, //2!
	-0.4f, 0.1f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //3!
	-0.4f, 0.3f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.5f, //4!
	-0.2f, 0.3f,  0.0f,  1.0f, 0.0f, 0.0f,  0.5f, 0.5f, //5!
	-0.2f, 0.1f,  0.0f,  0.0f, 1.0f, 0.0f,  0.5f, 0.0f, //6!
	0.1f, 0.4f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f, //7
	0.5f, -0.1f,  0.0f,  1.0f, 1.0f, 0.0f,  6.0f, 6.0f, //8
	0.4f, -0.1f,  0.0f,  1.0f, 1.0f, 0.0f,  0.6f, 0.6f, //9!
	0.4f, -0.4f,  0.0f,  1.0f, 1.0f, 0.0f,  0.6f, 0.0f, //10!
	-0.5f,-0.4f,  0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, //11!
	-0.5f, -0.1f,  0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //12
	0.1f, 0.1f,  0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //13!
	0.1f, -0.1f,  0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 0.6f, //14!
	0.1f, -0.4f,  0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //15!
	};
	unsigned int indices[] = {
		0, 1, 11,
		2, 3, 4,
		4, 2, 5,
		5, 12, 6,
		6, 7, 13,
		13, 8, 14,
		14, 8, 9,
		14, 12, 10,
		10, 12, 1
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Координатные атрибуты
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Цветовые атрибуты
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Атрибуты текстурных 
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// установка параметров наложения текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// установка параметров фильтрации текстуры
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//загрузка изображения, создание текстуры и генерирование мипмап - уровней
	int width, height, nrChannels;
	unsigned char* data = SOIL_load_image("Textures/texture.png", &width, &height, 0, SOIL_LOAD_RGB);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	SOIL_free_image_data(data);

	Shader ourShader("texture.vs", "texture.fs");

	// Цикл рендеринга
	while (!glfwWindowShouldClose(window)) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		ourShader.use();
		glBindVertexArray(VAO);
		glEnable(GL_DEPTH_TEST);
		glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);
		//обмен содержимым font и back буферов. Отслеживание собитий ввода-вывода
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	return 0;
}

