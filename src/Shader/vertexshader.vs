#version 330 core
layout (location = 0) in vec3 position;

uniform vec4 color;
out vec4 outColor;

void main(){

	outColor = color;
	gl_Position = vec4(position, 1.0);
}