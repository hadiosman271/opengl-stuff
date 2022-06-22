#include "window.h"

void error_callback(int code, const char *description) {
	fprintf(stderr, "error: %d:%s\n", code, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

int firstMouse = 1;
float lastX, lastY;

void cursor_callback(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = 0;
	}

	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	updateCamera(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
	zoom(yOffset);
}

GLFWwindow *window;

void init(void) {
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit()) {
		fprintf(stderr, "error: glfw init failed\n");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	extern unsigned SCR_WIDTH, SCR_HEIGHT;
	extern const char *WIN_NAME;
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WIN_NAME, NULL, NULL);
	if (!window) {
		fprintf(stderr, "error: window creation failed\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	
	if (!(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))) {
		fprintf(stderr, "error: glad init failed\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	
}

int running(void) {
	glfwSwapBuffers(window);
	glfwPollEvents();

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);	

	return !glfwWindowShouldClose(window);
}

int input(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

float getTime(void) {
	return glfwGetTime();
}

void finish(void) {
	glfwTerminate();
}
