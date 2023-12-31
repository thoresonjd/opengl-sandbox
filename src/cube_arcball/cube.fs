#version 330 core

in vec3 normal;
in vec2 texCoords;
in vec3 color;
in vec3 fragPos;

struct PointLight {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};
uniform PointLight light;
uniform vec3 viewPos;
uniform sampler2D texture;
uniform bool useBlinnPhongShading;

out vec4 fragColor;

vec3 calculateBlinnPhongShading() {
	// direction vectors
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	// ambient
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * light.ambient;
	
	// diffuse
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse;
	
	// specular
	float specularStrength = 1.0;
	int shininess = 32;
	float spec = pow(max(dot(viewDir, halfwayDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * light.specular;

	return ambient + diffuse + specular;
};

float calculateAttenuation() {
	float distance = length(light.position - fragPos);
	return 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
}

void main() {
	if (useBlinnPhongShading) {
		vec3 lighting = calculateBlinnPhongShading();
		float attenuation = calculateAttenuation();
		lighting *= attenuation;
		fragColor = vec4(lighting, 1.0) * texture(texture, texCoords);
	} else 
		fragColor = texture(texture, texCoords);
}