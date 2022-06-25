#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>
#include <cglm/cglm.h>

enum { X, Y, Z };

#define glCheckError() glCheckError_(__FILE__, __LINE__)

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

#endif
