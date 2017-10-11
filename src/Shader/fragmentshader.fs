#version 330 core

// in variables
in	vec4 v4_outColor;
in	vec2 v2_outTexCoord;

// out variables
out	vec4 v4_fragColor;

uniform sampler2D myTexture;

void main() {

	v4_fragColor = v4_outColor * texture(myTexture, v2_outTexCoord);

}