#ifndef SHADER_H
#define SHADER_H

#include "common.h"

unsigned createShaderProgram(Info *shader, unsigned count) {
	if (count < 1 || count > 6)
		return 0;

	unsigned *shaderBuf = (unsigned *) malloc(count * sizeof(unsigned));
	assert(shaderBuf);
	unsigned program = glCreateProgram();

	for (int i = 0; i < count; i++) {
		FILE *file = fopen(shader[i].path, "rb");
		if (!file) {
			fprintf(stderr, "Error loading shader at %s\n", shader[i].path);
			continue;
		}

		fseek(file, 0, SEEK_END);
		int len = ftell(file);
		assert(len > 0);
		fseek(file, 0, SEEK_SET);
		char *source = (char *) calloc(1, len);
		assert(source);
		assert(fread(source, 1, len, file) == len);
		fclose(file);

		unsigned Shader = glCreateShader(shader[i].type);
		glShaderSource(Shader, 1, (const char * const *) &source, (const int *) &len);
		glCompileShader(Shader);
		free(source);

		int success;
		char infoLog[512];

		glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(Shader, 512, NULL, infoLog);
			fprintf(stderr, "Error compiling shader:\n%s", infoLog);
			glDeleteShader(Shader);
			continue;
		}

		shaderBuf[i] = Shader;
		glAttachShader(program, Shader);
	}

	glLinkProgram(program);

	for (int i = 0; i < count; i++)
		glDeleteShader(shaderBuf[i]);
	free(shaderBuf);

	return program;
}

#endif