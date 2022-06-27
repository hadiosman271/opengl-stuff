#include <cglm/cglm.h>

#include "cubie.h"

enum { X, Y, Z };

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
		{ 1.0f, 0.3f, 0.0f },	// orange
		{ 1.0f, 0.8f, 0.0f },	// yellow
		{ 0.0f, 0.3f, 0.7f },	// blue
		{ 0.8f, 0.0f, 0.0f },	// red
		{ 1.0f, 1.0f, 1.0f },	// white
		{ 0.0f, 0.6f, 0.3f },	// green
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
