#ifndef RENDER_H
#define RENDER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.h"

void setup(void);
void update(void);
void draw(void);
int input(int key);
void processInput(void);
void cleanup(void);

enum {
	forward = GLFW_KEY_UP,
	backward = GLFW_KEY_DOWN,
	left = GLFW_KEY_LEFT,
	right = GLFW_KEY_RIGHT,
	zoom_in = GLFW_KEY_KP_ADD,
	zoom_out = GLFW_KEY_KP_SUBTRACT,
	zoom_reset = GLFW_KEY_KP_ENTER,
	record = GLFW_KEY_Q
};

#endif
