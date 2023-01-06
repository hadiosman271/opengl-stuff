#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <stdio.h>

#include "state.h"
#include "init.h"
#include "update.h"
#include "draw.h"

unsigned win_width = 800;
unsigned win_height = 600;
const char *win_name = "collision";

GLFWwindow *create_window(void);
void error_callback(int code, const char *description);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

int main(void) {
	State s;
	GLFWwindow *window = create_window();

	if (window == NULL)
		return -1;

	init(&s);

	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		update(&s);

		draw(&s);
	
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	finish(&s);
	
	glfwTerminate();

	return 0;
}

GLFWwindow *create_window(void) {
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		fprintf(stderr, "error: glfw init failed\n");
		return NULL;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow(win_width, win_height, win_name, NULL, NULL);
	if (!window) {
		fprintf(stderr, "error: window creation failed\n");
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);

	if (!(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))) {
		fprintf(stderr, "error: glad init failed\n");
		glfwTerminate();
		return NULL;
	}

	int count;
	GLFWmonitor** monitors = glfwGetMonitors(&count);
	const GLFWvidmode* mode = glfwGetVideoMode(monitors[0]);

	int monitor_x, monitor_y;
	glfwGetMonitorPos(monitors[0], &monitor_x, &monitor_y);

	glfwSetWindowPos(window,
		monitor_x + (mode->width - win_width) / 4,
		monitor_y + (mode->height - win_height) / 2);

	glfwShowWindow(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

	glfwDefaultWindowHints();

	return window;
}

void error_callback(int code, const char *description) {
	fprintf(stderr, "error: %d: %s\n", code, description);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, win_width = width, win_height = height);
}
