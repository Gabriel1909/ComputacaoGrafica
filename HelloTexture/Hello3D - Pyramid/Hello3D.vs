#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 texc;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 finalColor;
out vec3 scaledNormal;
out vec3 fragPos;
out vec2 texCoord;

void main() {
	gl_Position = projection * view * model * vec4(position, 1.0f);
	finalColor = color;
	scaledNormal = normal; // mat3(transpose(inverse(model))) * normal;
	fragPos = vec3(model * vec4(position, 1.0f));
	texCoord = vec2(texc.x,1-texc.y);
}