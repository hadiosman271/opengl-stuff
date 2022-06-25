#include "render.h"
#include "shader.h"
#include "shadertext.h"
#include "camera.h"
#include "cubie.h"

unsigned verts;
unsigned VBO, cubeVAO, lightVAO;
unsigned cubeProgram, lightProgram;
vec3 *cubePos;
vec3 vecThird = { 0.333, 0.333, 0.333 };
extern struct camera cam;
extern unsigned SCR_WIDTH, SCR_HEIGHT;

void setup(void) {
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	unsigned size;
	float *cube = cubie(&size);
	verts = size / (9 * sizeof(float));
	cubePos = cubiePos(cube[Z] * 2);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, cube, GL_STATIC_DRAW);
	free(cube);

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void *) 0);
	// colours
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void *) (3 * sizeof(float)));
	// normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void *) (6 * sizeof(float)));

	unsigned lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		9 * sizeof(float), (void *) 0);

	cubeProgram = createShaderProgram(cubeShaders, SHADERCOUNT);
	lightProgram = createShaderProgram(lightShaders, SHADERCOUNT);

	vec3 lightPos = { 1.2f, 1.0f, 2.0f };
	vec3 lightColour = { 1.0f, 1.0f, 1.0f };
	vec3 vecFifth = { 0.2f, 0.2f, 0.2f };
	vec3 vecHalf = { 0.5f, 0.5f, 0.5f };
	mat4 lmodel = GLM_MAT4_IDENTITY_INIT;
	glm_translate(lmodel, lightPos);
	glm_scale(lmodel, vecFifth);

	uniformVec3(cubeProgram, "light.ambient", vecFifth);
	uniformVec3(cubeProgram, "light.diffuse", vecHalf);
	uniformVec3(cubeProgram, "light.specular", lightColour);
	uniformVec3(cubeProgram, "light.pos", lightPos);

	uniformFloat(cubeProgram, "material.ambient", 0.8f);
	uniformFloat(cubeProgram, "material.diffuse", 2.0f);
	uniformFloat(cubeProgram, "material.specular", 1.5f);
	uniformFloat(cubeProgram, "material.shininess", 32.0f);

	uniformMat4(lightProgram, "model", lmodel);
	uniformVec3(lightProgram, "lightColour", lightColour);

	updateCamera(0, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void update(void) {
	processInput();

	mat4 view, proj;
	glm_perspective(cam.zoom, SCR_WIDTH / SCR_HEIGHT, 0.01f, 100.0f, proj);

	vec3 vecSum;
	glm_vec3_add(cam.pos, cam.front, vecSum);
	glm_lookat(cam.pos, vecSum, cam.up, view);

	uniformMat4(cubeProgram, "proj", proj);
	uniformMat4(cubeProgram, "view", view);
	uniformVec3(cubeProgram, "viewPos", cam.pos);

	uniformMat4(lightProgram, "proj", proj);
	uniformMat4(lightProgram, "view", view);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(cubeVAO);
	glUseProgram(cubeProgram);

	for (int i = 0; i < 27; i++) {
		mat4 model = GLM_MAT4_IDENTITY_INIT;
		glm_scale(model, vecThird);
		glm_translate(model, cubePos[i]);
		uniformMat4(cubeProgram, "model", model);

		glDrawArrays(GL_TRIANGLES, 0, verts);
	}

	glBindVertexArray(lightVAO);
	glUseProgram(lightProgram);

	glDrawArrays(GL_TRIANGLES, 0, verts);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(void) {	
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	float speed = cam.speed * deltaTime;

	// movement
	if (input(forward))
		glm_vec3_muladds(cam.front, speed, cam.pos);
	if (input(backward))	
		glm_vec3_muladds(cam.behind, speed, cam.pos);
	if (input(left))	
		glm_vec3_muladds(cam.left, speed, cam.pos);
	if (input(right))
		glm_vec3_muladds(cam.right, speed, cam.pos);

	// zooming
	if (input(zoom_in))
		cam.zoom -= speed;
	if (input(zoom_out))
		cam.zoom += speed;
	if (input(zoom_reset))
		cam.zoom = 45.0f;
		
	if (cam.zoom > 45.0f)
		cam.zoom = 45.0f;
	if (cam.zoom < 1.0f)
		cam.zoom = 1.0f;
}

void cleanup(void) {
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
}
