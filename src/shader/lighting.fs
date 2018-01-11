#version 410 core

uniform vec4 vec4_color;
out	vec4 v4_fragColor;

void main()
{
	v4_fragColor = vec4_color;
}