#ifndef COMMON_H
#define COMMON_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct info {
	char *path;
	unsigned type;
} Info;

#endif