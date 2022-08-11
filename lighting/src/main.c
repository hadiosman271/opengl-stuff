#include <string.h>

#include <stb_image.h>

#include "render.h"
#include "camera.h"

unsigned SCR_WIDTH = 800;
unsigned SCR_HEIGHT = 600;
const char *WIN_NAME = "lighting";

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

int input(int key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}

int main(void) {
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit()) {
		fprintf(stderr, "error: glfw init failed\n");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
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

	GLFWimage icon[1];
	if ((icon[0].pixels = stbi_load("res/icon.png", &icon[0].width, &icon[0].height, 0, 4)))
		glfwSetWindowIcon(window, 1, icon);
	else
		fprintf(stderr, "error: failed to load icon\n");
	stbi_image_free(icon[0].pixels);
	
	glfwSetWindowPos(window, 150, 300);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	
	setup();

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		update();

		draw();
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanup();

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

GLenum glCheckError_(const char *file, int line) {
	GLenum code;
	char message[30];

	while ((code = glGetError()) != GL_NO_ERROR) {
		switch (code) {
		case GL_INVALID_ENUM:
			strcpy(message, "INVALID_ENUM");
			break;
		case GL_INVALID_VALUE:
			strcpy(message, "INVALID_VALUE");
			break;
		case GL_INVALID_OPERATION:
			strcpy(message, "INVALID_OPERATION");
			break;
		case GL_OUT_OF_MEMORY:
			strcpy(message, "OUT_OF_MEMORY");
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			strcpy(message, "INVALID_FRAMEBUFFER_OPERATION");
			break;
		}
		
		fprintf(stderr, "%s | %s (line %d)\n",
			message, file, line);
	}
	
	return code;
}
