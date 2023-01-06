#include <glad/glad.h>

#include "draw.h"
#include "shader.h"

void draw(State *s) {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(s->shader);

	mat4 proj;
	glm_ortho(0.0, win_width, win_height, 0.0, -1.0, 1.0, proj);
	uniform_mat4(s->shader, "proj", proj);

	for (unsigned i = 0; i < s->object_count; i++) {
		glBindVertexArray(s->object[i].vao->id);
		mat4 model;
		model_matrix(s->object[i].pos, s->object[i].size, s->object[i].rotation, model);

		uniform_mat4(s->shader, "model", model);
		uniform_vec3(s->shader, "colour", s->object[i].colour);

		glDrawArrays(GL_TRIANGLE_FAN, 0, s->object[i].vao->verts);
	}
}

void model_matrix(vec2 pos, vec2 size, float rotation, mat4 model) {
	vec3 vec;
	glm_vec3_zero(vec);
	glm_mat4_identity(model);

	glm_vec2_copy(pos, vec);
	glm_translate(model, vec);

	glm_vec2_scale(size, 0.5, vec);
	glm_translate(model, vec);

	vec3 z_axis = { 0.0, 0.0, 1.0 };
	glm_rotate(model, glm_rad(rotation), z_axis);

	glm_vec2_scale(size, -0.5, vec);
	glm_translate(model, vec);

	glm_vec2_copy(size, vec);
	vec[2] = 1.0;
	glm_scale(model, vec);
}
