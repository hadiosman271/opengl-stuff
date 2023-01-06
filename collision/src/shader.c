#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

#include "shader.h"

unsigned load_shader(const char *vs_path, const char *fs_path) {
	unsigned program = glCreateProgram();
	unsigned shader_buf[2];

	struct shader {
		const char *path;
		unsigned type;
	} shaders[2] = {
		vs_path, GL_VERTEX_SHADER,
		fs_path, GL_FRAGMENT_SHADER
	};

	int success;
	char info_log[512];
	
	for (int i = 0; i < 2; i++) {
		FILE *file = fopen(shaders[i].path, "rb");
		if (!file) {
			fprintf(stderr, "Error loading shader at %s\n", shaders[i].path);
			continue;
		}

		fseek(file, 0, SEEK_END);
		int len = ftell(file);
		fseek(file, 0, SEEK_SET);
		char *source = malloc(len + 1);
		fread(source, 1, len, file);
		source[len] = '\0';
		fclose(file);

		unsigned shader = glCreateShader(shaders[i].type);
		shader_buf[i] = shader;
		glShaderSource(shader, 1, (const char * const *) &source, NULL);
		free(source);
		glCompileShader(shader);
	
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, info_log);
			fprintf(stderr, "Error compiling shader at %s:\n%s",
				shaders[i].path, info_log);
		} else
			glAttachShader(program, shader);
	}
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, info_log);
		fprintf(stderr, "Error linking shader program:\n%s", info_log);
	}
	
	for (int i = 0; i < 2; i++)
		glDeleteShader(shader_buf[i]);
	
	return program;

}

void uniform_mat4(unsigned program, const char *name, mat4 mat) {
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &mat[0][0]);
}

void uniform_vec3(unsigned program, const char *name, vec3 vec) {
	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, name), 1, &vec[0]);
}
