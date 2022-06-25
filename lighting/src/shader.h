#ifndef SHADER_H
#define SHADER_H

#include "common.h"

enum { SHADERCOUNT = 2 };
struct shaderInfo {
	unsigned type;
	const char *source;
};

unsigned createShaderProgram(struct shaderInfo shaders[], unsigned count);
void uniformInt(unsigned program, const char *name, int value);
void uniformFloat(unsigned program, const char *name, float value);
void uniformMat4(unsigned program, const char *name, mat4 mat);
void uniformVec3(unsigned program, const char *name, vec3 vec);

#endif
