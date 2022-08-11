#ifndef RENDER_H
#define RENDER_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "common.h"

void setup(void);
void update(void);
void processInput(void);
void draw(void);
void cleanup(void);

#endif
