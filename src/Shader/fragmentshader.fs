#version 330 core

uniform outColor;

out vec4 fragColor;

void main() {
	fragColor = outColor;
}