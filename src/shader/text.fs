#version 450 core

layout (location = 0) out vec4 v4_fragColor;

////////////////////////////
// in variables
////////////////////////////
in	vec2 v2_outTexCoord;

////////////////////////////
// uniform variables
////////////////////////////
uniform vec4 		v4_color;
uniform sampler2D 	Texture;

////////////////////////////
// entry point
////////////////////////////
void main() {

	v4_fragColor = texture(Texture, v2_outTexCoord).r * v4_color;
	
}
