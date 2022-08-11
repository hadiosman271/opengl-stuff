#include "shader.h"

unsigned createShaderProgram(Shader shaders[], unsigned count) {
	if (count < 1 || count > 6)
		return 0;

	unsigned program = glCreateProgram();
	unsigned *shader_buf = malloc(sizeof(unsigned [count]));
	
	int success;
	char info_log[512];
	
	for (int i = 0; i < count; i++) {
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
	
	for (int i = 0; i < count; i++)
		glDeleteShader(shader_buf[i]);
	free(shader_buf);
	
	return program;
}

void uniformInt(unsigned program, const char *name, int value) {
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, name), value);
}

void uniformFloat(unsigned program, const char *name, float value) {
	glUseProgram(program);
	glUniform1f(glGetUniformLocation(program, name), value);
}

void uniformMat4(unsigned program, const char *name, mat4 mat) {
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, &mat[0][0]);
}

void uniformVec3(unsigned program, const char *name, vec3 vec) {
	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, name), 1, &vec[0]);
}
