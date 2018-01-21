#version 410 core

uniform vec4 v4_color;
out	vec4 v4_fragColor;

void main()
{
	v4_fragColor = v4_color;
}