#version 330 core

// in variables
in	vec4 v4_outColor;
in	vec2 v2_outTexCoord;

//in 	bool boolean_outLight; 
in	vec3 v3_outNormal;
in	vec4 v4_outLightColor;
in	vec3 v3_outLightPosition;
in 	vec3 v3_outFragmentPosition;
in	vec3 v3_outCameraPosition;

// out variables
out	vec4 v4_fragColor;

uniform bool 		boolean_light;
uniform float		float_ambient;
uniform float		float_specular;
uniform sampler2D 	myTexture;

void main() {

	vec4 light = vec4(1,1,1,1);
	
	// If there are some lights...
	// Implement light attributes
	
	if (boolean_light) {
		// Ambient light
		//float ambientStrength = 0.1;
		vec4 ambient = v4_outLightColor * float_ambient;
	
		// Diffuse light
		vec3 norm = normalize(v3_outNormal);
		vec3 lightDirection = normalize(v3_outLightPosition - v3_outFragmentPosition);
		float diff = max(dot(norm, lightDirection), 0.0);
		vec4 diffuse = diff * v4_outLightColor;
	
		// Specular light
		//float specularStrength = 1.0;
		vec3 viewDirection = normalize(v3_outCameraPosition - v3_outFragmentPosition);
		vec3 reflectedDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), 256);
		vec4 specular = spec * v4_outLightColor * float_specular;
	
		// Final light
		light = ambient + diffuse + specular;
		light.w = 1.0;
	}
	
	v4_fragColor = light * v4_outColor * texture(myTexture, v2_outTexCoord);

}