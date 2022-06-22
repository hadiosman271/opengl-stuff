#version 330 core

in vec3 colour;
in vec2 texCoord;

out vec4 fragColour;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	fragColour = mix(texture(texture0, texCoord),
					 texture(texture1, texCoord), 0.5)
					 * vec4(colour, 1.0);
}