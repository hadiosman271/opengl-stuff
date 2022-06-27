#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "images.h"
#include "data.h"

void setIcon(void) {
	extern GLFWwindow *window;

	GLFWimage image[1] = {
		[0] = {
			.width = 64,
			.height = 64,
			.pixels = icon
		}
	};

	glfwSetWindowIcon(window, 1, image);
}

void loadTexture(void) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D,
			0, GL_RGBA, 500, 500,
			0, GL_RGBA, GL_UNSIGNED_BYTE, container);
	glGenerateMipmap(GL_TEXTURE_2D);
}
