#ifndef RENDER_H
#define RENDER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.h"

void setup(void);
void update(void);
void draw(void);
int input(int key);
float getTime(void);
void processInput(void);
void cleanup(void);

enum {
	forward = GLFW_KEY_W,
	backward = GLFW_KEY_S,
	left = GLFW_KEY_A,
	right = GLFW_KEY_D,
	zoom_in = GLFW_KEY_KP_ADD,
	zoom_out = GLFW_KEY_KP_SUBTRACT,
	zoom_reset = GLFW_KEY_KP_ENTER
};

#endif
