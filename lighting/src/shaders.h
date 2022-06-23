#ifndef SHADERS_H
#define SHADERS_H

#include "common.h"

enum { SHADERCOUNT = 2 };
struct shaderInfo {
	unsigned type;
	const char *source;
};

struct shaderInfo cubeShaders[SHADERCOUNT] = {
	[0] = {
	.type = GL_VERTEX_SHADER,
	.source =
		"#version 330 core\n"

		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aColour;"
		"layout (location = 2) in vec3 aNormal;"
		
		"out vec3 fragPos;"
		"out vec3 colour;"
		"out vec3 normal;"
		
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		
		"void main() {"
			"gl_Position = proj * view * model * vec4(aPos, 1.0);"
			"fragPos = vec3(model * vec4(aPos, 1.0));"
			"colour = aColour;"
			"normal = aNormal;"
		"}"
	},
	[1] = {
	.type = GL_FRAGMENT_SHADER,
	.source =
		"#version 330 core\n"

		"struct Material {"
			"float ambient;"
			"float diffuse;"
			"float specular;"
			"float shininess;"
		"};"

		"struct Light {"
			"vec3 pos;"
			"vec3 ambient;"
			"vec3 diffuse;"
			"vec3 specular;"
		"};"
		
		"in vec3 fragPos;"
		"in vec3 colour;"
		"in vec3 normal;"
		
		"out vec4 fragColour;"
		
		"uniform vec3 viewPos;"
		"uniform Material material;"
		"uniform Light light;"
		
		"void main() {"
			"vec3 ambient = clamp(material.ambient * light.ambient * colour, 0.0, 1.0);"
			
			"vec3 lightDir = normalize(light.pos - fragPos);"
			"vec3 norm = normalize(normal);"
			"vec3 diffuse = clamp(dot(norm, lightDir)"
				"* material.diffuse * light.diffuse * colour, 0.0, 1.0);"
			
			"vec3 viewDir = normalize(viewPos - fragPos);"
			"vec3 reflectDir = reflect(-lightDir, norm);"
			"vec3 specular = clamp(pow(max(dot("
				"viewDir, reflectDir), 0.0), material.shininess)"
				"* material.specular * light.specular * colour, 0.0, 1.0);"

			"vec3 result = ambient + diffuse + specular;"
			"fragColour = vec4(result, 1.0);"
		"}"
	}
};

struct shaderInfo lightShaders[SHADERCOUNT] = {
	[0] = {
	.type = GL_VERTEX_SHADER,
	.source =
		"#version 330 core\n"
		
		"layout (location = 0) in vec3 aPos;"
		
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		
		"void main() {"
			"gl_Position = proj * view * model * vec4(aPos, 1.0);"
		"}"
	},
	[1] = {
	.type = GL_FRAGMENT_SHADER,
	.source =
		"#version 330 core\n"
		
		"out vec4 fragColour;"
		
		"uniform vec3 lightColour;"
		
		"void main() {"
			"fragColour = vec4(lightColour, 1.0);"
		"}"
	}
};

unsigned createShaderProgram(struct shaderInfo shaders[], unsigned count) {
	unsigned program = glCreateProgram();
	unsigned *shaderBuf = malloc(sizeof(unsigned [count]));
	
	int success;
	char infoLog[512];
	
	for (int i = 0; i < count; i++) {
		unsigned shader = glCreateShader(shaders[i].type);
		shaderBuf[i] = shader;
		glShaderSource(shader, 1, &shaders[i].source, NULL);
		glCompileShader(shader);
	
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			fprintf(stderr, "error: shader compilation failed:\n%s", infoLog);
		} else
			glAttachShader(program, shader);
	}
	glLinkProgram(program);
	
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		fprintf(stderr, "error: shader program linking failed:\n%s", infoLog);
	}
	
	for (int i = 0; i < count; i++)
		glDeleteShader(shaderBuf[i]);
	free(shaderBuf);
	
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

#endif
