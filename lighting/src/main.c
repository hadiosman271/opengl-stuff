#include "window.h"
#include "shaders.h"
#include "camera.h"

unsigned SCR_WIDTH = 800;
unsigned SCR_HEIGHT = 600;
const char *WIN_NAME = "lighting";

float *cubie(unsigned *size);
vec3 *cubiePos(float shift);
void processInput(void);

int main(void) {
	init();

	unsigned cubeVAO, VBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	unsigned size;
	float *cube = cubie(&size);
	vec3 *cubePos = cubiePos(cube[Z] * 2);
	unsigned verts = size / (9 * sizeof(float));

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

	unsigned cubeProgram = createShaderProgram(cubeShaders, SHADERCOUNT);
	unsigned lightProgram = createShaderProgram(lightShaders, SHADERCOUNT);

	vec3 lightPos = { 1.2f, 1.0f, 2.0f };
	vec3 lightColour = { 1.0f, 1.0f, 1.0f };
	vec3 vec3Fifth = { 0.2f, 0.2f, 0.2f };
	float third = 1.0f / 3.0f;
	vec3 vec3Third = { third, third, third };
	vec3 vec3Half = { 0.5f, 0.5f, 0.5f };
	mat4 lmodel = GLM_MAT4_IDENTITY_INIT;
	glm_translate(lmodel, lightPos);
	glm_scale(lmodel, vec3Fifth);

	uniformVec3(cubeProgram, "light.ambient", vec3Fifth);
	uniformVec3(cubeProgram, "light.diffuse", vec3Half);
	uniformVec3(cubeProgram, "light.specular", lightColour);
	uniformVec3(cubeProgram, "light.pos", lightPos);
	
	uniformMat4(lightProgram, "model", lmodel);
	uniformVec3(lightProgram, "lightColour", lightColour);

	updateCamera(0, 0);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	while (running()) {
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

		// drawing starts here
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(cubeVAO);
		glUseProgram(cubeProgram);
		
		for (int i = 0; i < 27; i++) {
			if (i == 13) continue;
			mat4 model = GLM_MAT4_IDENTITY_INIT;
			glm_scale(model, vec3Third);
			glm_translate(model, cubePos[i]);
			uniformMat4(cubeProgram, "model", model);

			glDrawArrays(GL_TRIANGLES, 0, verts);
		}

		glBindVertexArray(lightVAO);
		glUseProgram(lightProgram);

		glDrawArrays(GL_TRIANGLES, 0, verts);
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	finish();

	exit(EXIT_SUCCESS);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(void) {	
	float currentFrame = getTime();
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

// this creates vertex data for 1 cubie
float *cubie(unsigned *size) {
	*size = sizeof(float [1188]);
	float *vert = malloc(*size);

	float ratio = 0.2f;
	vec3 pt[8] = { [0][X] = 0.5f };
	pt[0][Y] = pt[0][X];
	pt[0][Z] = (pt[0][X] + pt[0][Y]) * (1 + ratio) * 0.5f;

	// face (pt 0 to 3)
	for (int i = 1; i < 4; i++) {
		pt[i][X] = -pt[i - 1][Y];
		pt[i][Y] =  pt[i - 1][X];
		pt[i][Z] =  pt[i - 1][Z];
	}
	// top (pt 4, 5) and right (pt 6, 7) edges
	for (int i = 4; i < 8; i++) {
		int a = i < 6, b = !a, c = a ? 2 : 5;
		pt[i][a] =  pt[i - c][Z];
		pt[i][b] =  pt[i - c][b];
		pt[i][Z] = -pt[i - c][a];
	}

	vec3 colour[7] = {
		{ 0.0f, 0.3f, 0.7f },	// blue
		{ 1.0f, 0.8f, 0.0f },	// yellow
		{ 0.8f, 0.0f, 0.0f },	// red
		{ 0.0f, 0.6f, 0.3f },	// green
		{ 1.0f, 1.0f, 1.0f },	// white
		{ 1.0f, 0.3f, 0.0f },	// orange
		{ 0.2f, 0.2f, 0.2f }	// grey
	};

	int index[4][6] = {
		{ 0, 3, 2, 0, 1, 2 },	// face
		{ 5, 0, 1, 5, 4, 1 },	// top edge
		{ 6, 7, 3, 6, 0, 3 },	// right edge
		{ 0, 5, 6 },	// corner
	};

	int v = 0;
	for (int i = 0, one = 1; i < 6; i++) {
		for (int j = 0; j < 3; j++) {	// finding normal vector
			vec3 normal, sub1, sub2;
			glm_vec3_sub(pt[index[j][0]], pt[index[j][1]], sub1);
			glm_vec3_sub(pt[index[j][2]], pt[index[j][1]], sub2);
			glm_vec3_crossn(sub1, sub2, normal);

			for (int k = 0; k < 6; k++, v++) {	// copy and basement
				glm_vec3_copy(pt[index[j][k]], &vert[v * 9]);
				glm_vec3_copy(colour[!j ? i : 6], &vert[v * 9 + 3]);
				glm_vec3_copy(normal, &vert[v * 9 + 6]);
			}
		}
		i % 3 ? one *= -1 : 0;
		int a = i % 3 == 2, b = a ? -2 : 1;
		for (int j = 0; j < 8; j++) {	// rotating one face around
			vec3 temp;
			glm_vec3_copy(pt[j], temp);
			pt[j][X] = temp[i % 3];
			pt[j][Y] = temp[1 + one - a] *  one;
			pt[j][Z] = temp[(i % 3) + b] * -one;
		}
	}

	for (int i = 0; i < 8; i++) {	// same thing but with corners
		int a = i < 4, b = !a;
		vec3 normal, sub1, sub2;
		glm_vec3_sub(pt[index[3][a]], pt[index[3][2]], sub1);
		glm_vec3_sub(pt[index[3][b]], pt[index[3][2]], sub2);
		glm_vec3_crossn(sub1, sub2, normal);

		for (int j = 0; j < 3; j++, v++) {
			glm_vec3_copy(pt[index[3][j]], &vert[v * 9]);
			glm_vec3_copy(colour[6], &vert[v * 9 + 3]);
			glm_vec3_copy(normal, &vert[v * 9 + 6]);

			pt[index[3][j]][X] = -vert[v * 9 + Y];
			pt[index[3][j]][Y] =  vert[v * 9 + X];
			pt[index[3][j]][Z] =  vert[v * 9 + Z];
			i == 3 ? pt[index[3][j]][Z] *= -1 : 0;
		}
	}

	return vert;
}

// this creates position data for 27 cubies
vec3 *cubiePos(float shift) {
	vec3 *pos = malloc(sizeof(vec3 [27]));
	
	int p = 0;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++) {
				vec3 pt = {
					shift * (1 - i),
					shift * (1 - j),
					shift * (1 - k)
				};

				glm_vec3_copy(pt, pos[p++]);
			}

	return pos;
}
