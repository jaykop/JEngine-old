#version 330 core

// in variables
in	vec4 v4_outColor;
in	vec2 v2_outTexCoord;

in	vec3 v3_outNormal;
in	vec4 v4_outLightColor;
in	vec3 v3_outLightPosition;
in 	vec3 v3_outFragmentPosition;

// out variables
out	vec4 v4_fragColor;

uniform sampler2D myTexture;

void main() {

	// Ambient light
	float ambientConstant = 0.1;
	vec4 ambient = v4_outLightColor * ambientConstant;
	
	// Diffuse light
	vec3 norm = normalize(v3_outNormal);
	vec3 lightDirection = normalize(v3_outLightPosition 
									- v3_outFragmentPosition);
	float diff = max(dot(norm, lightDirection), 1);
	vec4 diffuse = diff * v4_outLightColor;
	
	// Final light
	vec4 light = ambient + diffuse;
	
	v4_fragColor = light * v4_outColor * texture(myTexture, v2_outTexCoord);

}