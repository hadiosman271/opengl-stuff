#include <string.h>

#include <stb_image.h>

#include "render.h"

unsigned scr_width = 800;
unsigned scr_height = 600;
const char *win_name = "2d";

void error_callback(int code, const char *description) {
	fprintf(stderr, "error: %d:%s\n", code, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, scr_width = width, scr_height = height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS)
		processInput(key);
}

GLFWwindow *window;

int main(void) {
	glfwSetErrorCallback(error_callback);
	
	if (!glfwInit()) {
		fprintf(stderr, "error: glfw init failed\n");
		exit(EXIT_FAILURE);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(scr_width, scr_height, win_name, NULL, NULL);
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
	glfwSetKeyCallback(window, key_callback);
	
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

int glCheckError_(const char *file, int line) {
	int code;
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
