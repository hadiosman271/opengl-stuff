#ifndef STATE_H
#define STATE_H

#include <cglm/cglm.h>

typedef struct vao {
	unsigned id, vbo, verts;
} Vao;

typedef struct object {
	Vao *vao;
	vec2 pos, vel, size;
	float rotation;
	vec3 colour;
} Object;

typedef struct state {
	Object *object;
	unsigned object_count;
	Vao *vao;
	unsigned vao_count;
	unsigned shader;
} State;

extern unsigned win_width, win_height;

#endif /* state.h */
