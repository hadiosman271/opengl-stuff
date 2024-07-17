#version 330 core

in vec3 fragPos;
in vec3 color;
in vec3 normal;

out vec4 fragColor;

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 pos;
};

uniform vec3 viewPos;
uniform Light light;

void main() {
	vec3 ambient = light.ambient * color;

	vec3 lightDir = normalize(light.pos - fragPos);
	vec3 norm = normalize(normal);
	vec3 diffuse = max(dot(norm, lightDir), 0.0)
		* light.diffuse * color;

	fragColor = vec4(ambient + diffuse, 1.0);
}
