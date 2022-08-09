#ifndef RENDER_H
#define RENDER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.h"

void setup(void);
void update(void);
void processInput(int key);
void draw(void);
void cleanup(void);

enum {
	up = GLFW_KEY_W,
	down = GLFW_KEY_S,
	left = GLFW_KEY_A,
	right = GLFW_KEY_D,
};

#endif
