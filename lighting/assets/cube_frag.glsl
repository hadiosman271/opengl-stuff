#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

out vec4 fragColour;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 pos;
};

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() {
	vec3 materialDiffuse = vec3(texture(material.diffuse, texCoord));
	vec3 materialSpecular = vec3(texture(material.specular, texCoord));

	vec3 ambient = light.ambient * materialDiffuse;

	vec3 lightDir = normalize(light.pos - fragPos);
	vec3 norm = normalize(normal);
	vec3 diffuse = max(dot(norm, lightDir), 0.0)
		* light.diffuse * materialDiffuse;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 specular = pow(max(dot(
		viewDir, reflectDir), 0.0), material.shininess)
		* light.specular * materialSpecular;

	fragColour = vec4(ambient + diffuse + specular, 1.0);
}
