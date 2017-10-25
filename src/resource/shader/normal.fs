#version 330 core

////////////////////////////
// structs
////////////////////////////
struct Material{
	vec4 m_ambient;		// Reflected color
	vec4 m_diffuse;		// Desired obj's color
	vec4 m_specular;	// specular highlighted color
	float m_shininess;	// impacts the specular light
};

struct Light {
	vec3 m_position;
	vec4 m_ambient;
	vec4 m_diffuse;
	vec4 m_specular;
};

////////////////////////////
// in variables
////////////////////////////
in	vec4 v4_outColor;
in	vec2 v2_outTexCoord;

in	vec3 v3_outNormal;
in	vec4 v4_outLightColor;
in 	vec3 v3_outFragmentPosition;
in	vec3 v3_outCameraPosition;

////////////////////////////
// out variables
////////////////////////////
out	vec4 v4_fragColor;

////////////////////////////
// uniform variables
////////////////////////////
uniform bool 		boolean_light;
uniform float		float_ambient;
uniform float		float_specular;
uniform sampler2D 	Texture;
uniform Material 	material;
uniform Light		light;

////////////////////////////
// function declarations
////////////////////////////
void LightingEffect(inout vec4 _light);

////////////////////////////
// entry point
////////////////////////////
void main() {

	vec4 defaultLight = vec4(1,1,1,1);
	
	// If there are some lights...
	// Implement light attributes
	if (boolean_light)
		LightingEffect(defaultLight);
	
	v4_fragColor = defaultLight * v4_outColor * texture(Texture, v2_outTexCoord);

}

////////////////////////////
// function bodies
////////////////////////////
void LightingEffect(inout vec4 _light) {

	// Ambient light
	vec4 ambient = light.m_ambient * material.m_ambient; 
	
	// Diffuse light
	vec3 norm = normalize(v3_outNormal);
	vec3 lightDirection = normalize(light.m_position - v3_outFragmentPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec4 diffuse = light.m_diffuse * (diff * material.m_diffuse);
	
	// Specular light
	vec3 viewDirection = normalize(v3_outCameraPosition - v3_outFragmentPosition);
	vec3 reflectedDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.m_shininess);
	vec4 specular = light.m_specular * (spec * material.m_specular); 
	
	// Final light
	_light = v4_outLightColor * (ambient + diffuse + specular);
	_light.w = 1.0;
}