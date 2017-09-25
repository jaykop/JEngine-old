#version 330 core
layout (location = 0) in vec3 position;

out vec4 outColor;

void main(){
	gl_Position = vec4(position, 1.0);
	outColor = vec4(1.0, 0, 0, 1.0);
}