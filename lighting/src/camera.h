#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

struct camera {
	float speed;
	float pitch, yaw;
	float zoom;
	float sensitivity;
	vec3 pos;
	vec3 front, behind;
	vec3 right, left;
	vec3 up;
};

void updateCamera(float xOffset, float yOffset);
void zoom(float amount);

#endif
