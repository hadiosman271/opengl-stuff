#include "init.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

#include "cubes.h"

const unsigned SCR_WIDTH = 640;
const unsigned SCR_HEIGHT = 480;

const char *WIN_NAME = "spinning cubes";

extern Camera cam;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void) {
	GLFWwindow *window = initContext();

	unsigned VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	extern float vertices[];
	extern vec3 cubePositions[];
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	// colour
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);

	Info shaders[] = {
		{ .path = "res/shaders/vertex.vs", .type = GL_VERTEX_SHADER },
		{ .path = "res/shaders/fragment.fs", .type = GL_FRAGMENT_SHADER }
	};
	unsigned shaderProgram = createShaderProgram(shaders, 2);

	Info images[] = {
		{ .path = "res/img/image0.png", .type = GL_RGBA },
		{ .path = "res/img/image1.png", .type = GL_RGBA }
	};
	unsigned *textures = createTextures(images, 2);

	glUseProgram(shaderProgram);
	unsigned modelLoc = glGetUniformLocation(shaderProgram, "model");
	unsigned viewLoc = glGetUniformLocation(shaderProgram, "view");
	unsigned projLoc = glGetUniformLocation(shaderProgram, "proj");
	
	glUniform1i(glGetUniformLocation(shaderProgram, "texture0"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 1);
	
	glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		
		vec3 vecSum;
		glm_vec3_add(cam.pos, cam.front, vecSum);
		mat4 view;
		glm_lookat(cam.pos, vecSum, cam.up, view);
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
		
		mat4 proj;
		glm_perspective(glm_rad(cam.zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f, proj);
		
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, &proj[0][0]);
		
		// drawing starts here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		for (int i = 0; i < 10; i++) {
			mat4 model = GLM_MAT4_IDENTITY_INIT;
			
			glm_scale(model, (vec3) { 0.5f, 0.5f, 0.5f });
			glm_translate(model, cubePositions[i]);
			glm_rotate(model, glfwGetTime() * glm_rad(20.0f * i + 1), (vec3) { 1.0f, 0.3f, 0.5f });
			
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	// movement
	float speed = cam.speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		glm_vec3_muladds(cam.front, speed, cam.pos);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		glm_vec3_muladds(cam.behind, speed, cam.pos);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		glm_vec3_muladds(cam.left, speed, cam.pos);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		glm_vec3_muladds(cam.right, speed, cam.pos);

	// zooming
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		cam.zoom -= speed * 10;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		cam.zoom += speed * 10;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		cam.zoom = 45.0f;
		
	if (cam.zoom > 45.0f)
		cam.zoom = 45.0f;
	if (cam.zoom < 1.0f)
		cam.zoom = 1.0f;
}
