#version 330 core

layout (location = 0) in vec2 in_pos;

uniform mat4 model, proj;

void main() {
	gl_Position = proj * model * vec4(in_pos, 0.0, 1.0);
}
