#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <cglm/cglm.h>

#define glCheckError() glCheckError_(__FILE__, __LINE__)

int glCheckError_(const char *file, int line);

enum { X, Y, Z };

#endif
