struct shaderInfo cubeShaders[SHADERCOUNT] = {
	[0] = {
	.type = GL_VERTEX_SHADER,
	.source =
		"#version 330 core\n"

		"layout (location = 0) in vec3 aPos;"
		"layout (location = 1) in vec3 aNormal;"
		"layout (location = 2) in vec2 aTexCoord;"
		
		"out vec3 fragPos;"
		"out vec3 normal;"
		"out vec2 texCoord;"
		
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		
		"void main() {"
			"gl_Position = proj * view * model * vec4(aPos, 1.0);"
			"fragPos = vec3(model * vec4(aPos, 1.0));"
			"normal = aNormal;"
			"texCoord = aTexCoord;"
		"}"
	},
	[1] = {
	.type = GL_FRAGMENT_SHADER,
	.source =
		"#version 330 core\n"

		"in vec3 fragPos;"
		"in vec3 normal;"
		"in vec2 texCoord;"
		
		"out vec4 fragColour;"

		"struct Material {"
			"sampler2D diffuse;"
			"vec3 specular;"
			"float shininess;"
		"};"

		"struct Light {"
			"vec3 ambient;"
			"vec3 diffuse;"
			"vec3 specular;"
			"vec3 pos;"
		"};"
		
		"uniform vec3 viewPos;"
		"uniform Material material;"
		"uniform Light light;"
		
		"void main() {"
			"vec3 colour = vec3(texture(material.diffuse, texCoord));"

			"vec3 ambient = light.ambient * colour;"
			
			"vec3 lightDir = normalize(light.pos - fragPos);"
			"vec3 norm = normalize(normal);"
			"vec3 diffuse = max(dot(norm, lightDir), 0.0)"
				"* light.diffuse * colour;"
			
			"vec3 viewDir = normalize(viewPos - fragPos);"
			"vec3 reflectDir = reflect(-lightDir, norm);"
			"vec3 specular = pow(max(dot("
				"viewDir, reflectDir), 0.0), material.shininess)"
				"* light.specular * material.specular;"
	
			"fragColour = vec4(ambient + diffuse + specular, 1.0);"
		"}"
	}
};

struct shaderInfo lightShaders[SHADERCOUNT] = {
	[0] = {
	.type = GL_VERTEX_SHADER,
	.source =
		"#version 330 core\n"
		
		"layout (location = 0) in vec3 aPos;"
		
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 proj;"
		
		"void main() {"
			"gl_Position = proj * view * model * vec4(aPos, 1.0);"
		"}"
	},
	[1] = {
	.type = GL_FRAGMENT_SHADER,
	.source =
		"#version 330 core\n"
		
		"out vec4 fragColour;"
		
		"uniform vec3 lightColour;"
		
		"void main() {"
			"fragColour = vec4(lightColour, 1.0);"
		"}"
	}
};
