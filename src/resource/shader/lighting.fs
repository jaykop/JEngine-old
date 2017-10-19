#version 330 core

// uniform vectors
uniform vec4 v4_sampleColor;
uniform vec4 v4_lightColor;

out vec4 FragColor;

void main()
{
	//vec4 finalColor = .xyz;
	FragColor = (v4_sampleColor*v4_lightColor);
}