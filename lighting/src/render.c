#include "render.h"
#include "verts.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"

extern vec3 cubePos[];
extern struct camera cam;
extern unsigned SCR_WIDTH, SCR_HEIGHT;

unsigned verts, VBO, cubeVAO, lightVAO;
unsigned cubeProgram, lightProgram;
unsigned texture;

vec3 vec3fifth = { 0.2f, 0.2f, 0.2f };
vec3 vec3half  = { 0.5f, 0.5f, 0.5f };
vec3 vec3one   = { 1.0f, 1.0f, 1.0f };

void setup(void) {
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	unsigned size;
	float *cube = box(&size);
	verts = size / (8 * sizeof(float));

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, cube, GL_STATIC_DRAW);
	free(cube);

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void *) 0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void *) (3 * sizeof(float)));
	// texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void *) (6 * sizeof(float)));

	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		8 * sizeof(float), (void *) 0);
	
	Shader cubeShaders[2] = {
		{ .path = "assets/cube_vert.glsl", .type = GL_VERTEX_SHADER },
		{ .path = "assets/cube_frag.glsl", .type = GL_FRAGMENT_SHADER }
	};
	Shader lightShaders[2] = {
		{ .path = "assets/light_vert.glsl", .type = GL_VERTEX_SHADER },
		{ .path = "assets/light_frag.glsl", .type = GL_FRAGMENT_SHADER }
	};
	cubeProgram = createShaderProgram(cubeShaders, 2);
	lightProgram = createShaderProgram(lightShaders, 2);

	Image images[2] = {
		{ .path = "assets/diffuse.png", .format = GL_RGBA },
		{ .path = "assets/specular.png", .format = GL_RGBA }
	};
	unsigned *texture = createTextures(images, 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture[1]);

	uniformVec3(cubeProgram, "light.ambient", vec3fifth);
	uniformVec3(cubeProgram, "light.diffuse", vec3half);
	uniformVec3(cubeProgram, "light.specular", vec3one);
	
	uniformInt(cubeProgram, "material.diffuse", 0); // texture 0
	uniformInt(cubeProgram, "material.specular", 1); // texture 1
	uniformFloat(cubeProgram, "material.shininess", 32.0f);

	uniformVec3(lightProgram, "lightColour", vec3one);

	updateCamera(0, 0);	

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void update(void) {
	processInput();
	
	float time = glfwGetTime();

	vec3 lightPos = {
		cos(time) * 3.0f - 1.0f,
		sin(time) * 3.0f + 1.5f,
		sin(time * 0.5f) * 6.0f + 7.0f
	};

	mat4 model = GLM_MAT4_IDENTITY_INIT;
	glm_translate(model, lightPos);
	glm_scale(model, vec3fifth);

	vec3 vecSum;
	glm_vec3_add(cam.pos, cam.front, vecSum);

	mat4 view, proj;
	glm_lookat(cam.pos, vecSum, cam.up, view);
	glm_perspective(cam.zoom, SCR_WIDTH / SCR_HEIGHT,
			0.01f, 100.0f, proj);

	uniformMat4(cubeProgram, "view", view);
	uniformMat4(cubeProgram, "proj", proj);
	uniformVec3(cubeProgram, "viewPos", cam.pos);
	uniformVec3(cubeProgram, "light.pos", lightPos);

	uniformMat4(lightProgram, "model", model);
	uniformMat4(lightProgram, "view", view);
	uniformMat4(lightProgram, "proj", proj);
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(cubeVAO);	
	glUseProgram(cubeProgram);	

	for (int i = 0; i < 10; i++) {
		vec4 rot;
		glm_vec4(cubePos[i], 1.0f, rot);
		mat4 model = GLM_MAT4_IDENTITY_INIT;

		glm_translate(model, cubePos[i]);
		glm_rotate(model, glfwGetTime(), rot);

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

//	if (input(record))
//		printf("pos = (%f, %f, %f)\npitch = %f, yaw = %f\n",
//			cam.pos[X], cam.pos[Y], cam.pos[Z], cam.pitch, cam.yaw);

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
	glDeleteTextures(2, &texture);
}
