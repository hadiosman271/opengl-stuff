#ifndef INIT_H
#define INIT_H

#include "common.h"

void error_callback(int code, const char *description);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

extern const unsigned SCR_WIDTH;
extern const unsigned SCR_HEIGHT;

extern const char *WIN_NAME;

GLFWwindow *initContext(void) {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "Error: failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_NAME, NULL, NULL);
	if (!window) {
		fprintf(stderr, "Error: Failed to create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		fprintf(stderr, "Error: failed to initialize GLAD\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	return window;
}

void error_callback(int code, const char *description) {
	fprintf(stderr, "Error: code %d: %s\n", code, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

#endif