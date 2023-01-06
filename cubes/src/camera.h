#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

typedef struct camera {
	float speed;
	float pitch, yaw;
	float zoom;
	float sensitivity;
	vec3 pos;
	vec3 front, behind;
	vec3 right, left;
	vec3 up;
} Camera;

Camera cam = {
	.speed = 2.5f,
	.pitch = 0.0f, .yaw = -90.0f,
	.zoom = 45.0f,
	.sensitivity = 0.1f,
	.pos    = {  0.0f, 0.0f,  3.0f },
	.front  = {  0.0f, 0.0f, -1.0f },
	.behind = {  0.0f, 0.0f,  1.0f },
	.right  = {  1.0f, 0.0f,  0.0f },
	.left   = { -1.0f, 0.0f,  0.0f },
	.up     = {  0.0f, 1.0f,  0.0f }
};

int firstMouse = 1;
float lastX, lastY;

void mouse_callback(GLFWwindow *window, double xPos, double yPos) {
	if (firstMouse) {
		lastX = xPos;
		lastY = yPos;
		firstMouse = 0;
	}
	
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;
	
	cam.pitch += yOffset * cam.sensitivity;
	cam.yaw += xOffset * cam.sensitivity;
	
	if (cam.pitch > 89.9f)
		cam.pitch = 89.9f;
	if (cam.pitch < -89.9f)
		cam.pitch = -89.9f;
		
	cam.front[0] = cos(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
	cam.front[1] = sin(glm_rad(cam.pitch));
	cam.front[2] = sin(glm_rad(cam.yaw)) * cos(glm_rad(cam.pitch));
	glm_vec3_normalize(cam.front);
	
	glm_vec3_negate_to(cam.front, cam.behind);
	glm_vec3_crossn(cam.front, cam.up, cam.right);
	glm_vec3_negate_to(cam.right, cam.left);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
	cam.zoom -= (float) yOffset;
}

#endif