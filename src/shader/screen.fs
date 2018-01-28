#version 410 core

in vec2 v2_outTexCoord;
out	vec4 v4_fragColor;

uniform sampler2D Framebuffer;

void main()
{
	vec2 texCoord = v2_outTexCoord;
	texCoord.y = 1 - texCoord.y;
	v4_fragColor = texture(Framebuffer, texCoord);
}