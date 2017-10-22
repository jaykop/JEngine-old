#version 330 core

in vec4 lightColor;
in vec4 objectColor;

out vec4 FragColor;

void main()
{
	//vec4 finalColor = .xyz;
	float ambientStrength = 0.1;
	
	vec4 ambient = ambientStrength*lightColor;
	
	vec4 result = lightColor ;
	FragColor = result;
}