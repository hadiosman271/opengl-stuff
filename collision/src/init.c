#include <glad/glad.h>

#include <stdlib.h>
#include <time.h>

#include "init.h"
#include "shader.h"

enum { X, Y };
enum { R, G, B };

void init(State *s) {
	s->shader = load_shader("res/vertex.vs", "res/fragment.fs");

	s->vao_count = 2;
	s->vao = calloc(1, sizeof(Object [s->vao_count]));
	init_square_vao(&s->vao[0]);
	init_circle_vao(&s->vao[1]);
	Vao *square = &s->vao[0], *circle = &s->vao[1];

	s->object_count = 6;
	s->object = calloc(1, sizeof(Object [s->object_count]));

	s->object[0].vao = square;

	for (unsigned i = 1; i < s->object_count; i++)
		s->object[i].vao = circle;

	srand((unsigned) time(NULL));

	vec2 pos, vel, size;
	vec3 colour;

	for (unsigned i = 0; i < s->object_count; i++) {
		size[X] = 25.0;
		size[Y] = 25.0;
		glm_vec2_copy(size, s->object[i].size);

		pos[X] = s->object[i].size[X] + rand() % (int) (win_width - 2 * s->object[i].size[X]);
		pos[Y] = s->object[i].size[Y] + rand() % (int) (win_height - 2 * s->object[i].size[Y]);
		glm_vec2_copy(pos, s->object[i].pos);

		vel[X] = (float) (rand() % 50) / 10;
		vel[Y] = (float) (rand() % 50) / 10;
		glm_vec2_copy(vel, s->object[i].vel);

		colour[R] = (float) (rand() % 255) / 255;
		colour[G] = (float) (rand() % 255) / 255;
		colour[B] = (float) (rand() % 255) / 255;
		glm_vec3_copy(colour, s->object[i].colour);
		s->object[i].rotation = 0;
	}

	glm_vec2_scale(s->object[1].size, 4.0, s->object[0].size);

	glClearColor(0.1, 0.1, 0.1, 1.0);
}

void init_square_vao(Vao *vao) {
	vao->verts = 4;
	float vertex_data[] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0
	};

	glGenVertexArrays(1, &vao->id);
	glGenBuffers(1, &vao->vbo);

	glBindVertexArray(vao->id);

	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init_circle_vao(Vao *vao) {
	unsigned n = 3;
	vao->verts = 360 / n;
	float *vertex_data = malloc(sizeof(float [vao->verts]));

	for (unsigned i = 0; i < vao->verts; i += 2) {
		vertex_data[i] = cos(glm_rad(i * n));
		vertex_data[i + 1] = sin(glm_rad(i * n));
	}

	glGenVertexArrays(1, &vao->id);
	glGenBuffers(1, &vao->vbo);

	glBindVertexArray(vao->id);

	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float [vao->verts]), vertex_data, GL_STATIC_DRAW);
	free(vertex_data);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *) 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void finish(State *s) {
	for (unsigned i = 0; i < s->vao_count; i++) {
		glDeleteVertexArrays(1, &s->vao[i].id);
		glDeleteBuffers(1, &s->vao[i].vbo);
	}

	free(s->vao);
	free(s->object);
}
