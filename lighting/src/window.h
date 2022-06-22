#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

enum {
	forward = GLFW_KEY_W,
	backward = GLFW_KEY_S,
	left = GLFW_KEY_A,
	right = GLFW_KEY_D,
	zoom_in = GLFW_KEY_KP_ADD,
	zoom_out = GLFW_KEY_KP_SUBTRACT,
	zoom_reset = GLFW_KEY_KP_ENTER
};

void init(void);
int running(void);
int input(int key);
float getTime(void);
void finish(void);
void updateCamera(float xOffset, float yOffset);
void zoom(float yOffset);

#endif
