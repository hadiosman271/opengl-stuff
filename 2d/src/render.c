#include "render.h"
#include "shader.h"
#include "texture.h"

extern unsigned scr_width, scr_height;

unsigned VBO, VAO, EBO;
unsigned program, *texture;

void setup(void) {
	float vertices[] = {
		// x, y			// s, t
		 0.1f,  0.1f,	1.0f, 1.0f,
		-0.1f,  0.1f,	0.0f, 1.0f,
		-0.1f, -0.1f,	0.0f, 0.0f,
		 0.1f, -0.1f,	1.0f, 0.0f
	};

	unsigned indices[] = {
		0, 1, 2,
		0, 3, 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(float), (void *) 0);
	// texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		4 * sizeof(float), (void *) (2 * sizeof(float)));

	Shader shaders[2] = {
		{ .path = "res/vertex.glsl", .type = GL_VERTEX_SHADER },
		{ .path = "res/fragment.glsl", .type = GL_FRAGMENT_SHADER }
	};
	program = createShaderProgram(shaders, 2);

	Image images[1] = {
		{ "res/mario.png", GL_RGBA }
	};
	texture = createTextures(images, 1);

	glUniform1i(glGetUniformLocation(program, "texture0"), 0);	

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float x = 0.0f;
float y = 0.0f;

void update(void) {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	mat4 proj;
	glm_ortho_default((float) scr_width / scr_height, proj);
	uniformMat4(program, "proj", proj);

	vec3 pos = { x, y, 0.0f };
	mat4 model = GLM_MAT4_IDENTITY_INIT;

	glm_translate(model, pos);
	uniformMat4(program, "model", model);
}

void processInput(int key) {
	float speed = 2.5f * deltaTime;

	// movement
	if (key == up)
		y += speed;
	if (key == down)
		y -= speed;
	if (key == left)
		x -= speed;
	if (key == right)
		x += speed;
}

void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAO);
	glUseProgram(program);
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}
