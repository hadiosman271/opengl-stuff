#include <cglm/struct.h>

#include "update.h"

enum { X, Y };

int colliding(Object *square, Object *circle);

void update(State *s) {
	Object *circle, *square = &s->object[0];

	for (unsigned i = 1; i < s->object_count; i++) {
		circle = &s->object[i];
		if (circle->pos[X] <= circle->size[X]
			|| circle->pos[X] >= win_width - circle->size[X])
			circle->vel[X] *= -1;
		if (circle->pos[Y] <= circle->size[Y]
			|| circle->pos[Y] >= win_height - circle->size[Y])
			circle->vel[Y] *= -1;

		if (colliding(square, circle))
			glm_vec2_negate(circle->vel);

		glm_vec2_add(circle->pos, circle->vel, circle->pos);
	}
}

int colliding(Object *square, Object *circle) {
	vec2 p, center;
	glm_vec2_scale(square->size, 0.5, p);
	glm_vec2_add(square->pos, p, center);
	glm_vec2_sub(circle->pos, center, p);
	glm_vec2_clamp(p, -square->size[X] / 2, square->size[X] / 2);
	glm_vec2_add(center, p, p);
	float distance = glm_vec2_distance(circle->pos, p);

	return distance <= circle->size[X];
}
