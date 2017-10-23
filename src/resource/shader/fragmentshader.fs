#version 330 core

// in variables
in	vec4 v4_outColor;
in	vec2 v2_outTexCoord;

in	vec3 v3_outNormal;
in	vec4 v4_outLightColor;
in	vec3 v3_outLightPosition;
in 	vec3 v3_outFragmentPosition;
in	vec3 v3_outCameraPosition;

// out variables
out	vec4 v4_fragColor;

uniform sampler2D myTexture;

void main() {

	// Ambient light
	float ambientStrength = 0.1;
	vec4 ambient = v4_outLightColor * ambientStrength;
	
	// Diffuse light
	vec3 norm = normalize(v3_outNormal);
	vec3 lightDirection = normalize(v3_outLightPosition - v3_outFragmentPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec4 diffuse = diff * v4_outLightColor;
	
	// Specular light
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(v3_outCameraPosition - v3_outFragmentPosition);
	vec3 reflectedDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), 32);
	vec4 specular = spec * specularStrength * v4_outLightColor;
	
	// Final light
	vec4 light = ambient + diffuse + specular;
	light.w = 1.0;
	
	v4_fragColor = light * v4_outColor * texture(myTexture, v2_outTexCoord);

}