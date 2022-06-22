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
} cam = {
	.speed = 2.5f,
	.pitch = -18.0f, .yaw = -15.0f,
	.zoom = 45.0f,
	.sensitivity = 0.1f,
	.pos = { -3.0f, 1.3f, 1.5f },
	.up = { 0.0f, 1.0f, 0.0f }
};

void updateCamera(float xOffset, float yOffset) {
	cam.pitch += yOffset * cam.sensitivity;
	cam.yaw += xOffset * cam.sensitivity;
	
	if (cam.pitch > 89.0f)
		cam.pitch = 89.0f;
	if (cam.pitch < -89.0f)
		cam.pitch = -89.0f;
	
	// creating camera vectors from pitch and yaw
	cam.front[X] = cos(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
	cam.front[Y] = sin(glm_rad(cam.pitch));
	cam.front[Z] = sin(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
	glm_vec3_normalize(cam.front);
	
	glm_vec3_crossn(cam.front, cam.up, cam.right);
	glm_vec3_negate_to(cam.front, cam.behind);
	glm_vec3_negate_to(cam.right, cam.left);
}

void zoom(float yOffset) {
	cam.zoom -= yOffset * cam.sensitivity;
}

#endif
