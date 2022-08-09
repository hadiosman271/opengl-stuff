#version 330 core

in vec2 texCoords;

out vec4 fragColour;

uniform sampler2D texture0;

void main() {
	fragColour = texture(texture0, texCoords);
}
