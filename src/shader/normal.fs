#version 330 core

#define MAX_ARRAY 128

////////////////////////////
// structs
////////////////////////////
struct Material{
	sampler2D m_diffuse;
	sampler2D m_specular;	// specular highlighted color
	float m_shininess;		// impacts the specular light
};

struct Light {

	int m_type;
	vec3 m_position;
	vec3 m_direction;
	
	vec4 m_ambient;
	vec4 m_diffuse;
	vec4 m_specular;
	
	float m_constant;
	float m_linear;
	float m_quadratic;
	
	float m_cutOff;
	float m_outerCutOff;
};

struct Effect {
	int m_blur;
	int 
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
uniform 

////////////////////////////
// function declarations
////////////////////////////
void LightingEffect(inout vec4 _light);

////////////////////////////
// entry point
////////////////////////////
void main() {

	vec4 finalTexture = vec4(1,1,1,1);
	
	// If there are some lights...
	// Implement light attributes
	if (boolean_light)
		LightingEffect(finalTexture);
		
	else
		finalTexture = texture(Texture, v2_outTexCoord);
	
	v4_fragColor = finalTexture * v4_outColor;

}

////////////////////////////
// function bodies
////////////////////////////
void LightingEffect(inout vec4 _light) {
		
	vec3 	lightDirection;
	float 	attenuation = 1.f;
	vec3 	gap = light.m_position - v3_outFragmentPosition;
	float 	theta = 0.f;
	
	// Driectional light
	if (light.m_type == 1)
		lightDirection = normalize(-light.m_direction);

	else {
		lightDirection = normalize(gap);
	
		// Spotlight
		if (light.m_type == 2) {
			theta = dot(lightDirection, normalize(-light.m_direction));
		}
		
		// Pointlight
		else if (light.m_type == 3) {
			float distance = length(gap);
			attenuation = 1.0 / (light.m_constant + light.m_linear * distance + light.m_quadratic * (distance * distance));
		}
	}	
	
	// Ambient light
	vec4 ambient = light.m_ambient * vec4(texture(material.m_diffuse, v2_outTexCoord)); 
	
	// Diffuse light
	vec3 norm = normalize(v3_outNormal);
	
	float diff = max(dot(norm, lightDirection), 0.0);
	vec4 diffuse = light.m_diffuse * vec4(diff * texture(material.m_diffuse, v2_outTexCoord)); 
	
	// Specular light
	vec3 viewDirection = normalize(v3_outCameraPosition - v3_outFragmentPosition);
	vec3 reflectedDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.m_shininess);
	vec4 specular = light.m_specular * vec4(spec * texture(material.m_specular, v2_outTexCoord)); 
	
	// Smooth spotlight
	if (light.m_type == 2) {
		float epsilon = light.m_cutOff - light.m_outerCutOff;
		float intensity = clamp((theta - light.m_outerCutOff) / epsilon, 0.0, 1.0);
		diffuse *= intensity;
		specular *= intensity;
	}
	
	// Final light
	_light = v4_outLightColor * ((ambient + diffuse + specular) * attenuation);
	_light.w = 1.0;
}