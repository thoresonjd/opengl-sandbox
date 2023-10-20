#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 3) in vec4 aColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 normal;
out vec3 color;
out vec3 fragPos;
void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(model))); 
	normal = normalize(normalMatrix * aNorm);
	color = aColor.rgb;
	fragPos = vec3(model * vec4(aPos, 1.0));
}