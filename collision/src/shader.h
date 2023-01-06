#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>

unsigned load_shader(const char *vs_path, const char *fs_path);
void uniform_mat4(unsigned program, const char *name, mat4 mat);
void uniform_vec3(unsigned program, const char *name, vec3 vec);

#endif /* shader.h */
