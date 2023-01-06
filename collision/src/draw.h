#ifndef DRAW_H
#define DRAW_H

#include "state.h"

void draw(State *s);
void model_matrix(vec2 pos, vec2 size, float rotation, mat4 model);

#endif /* draw.h */
