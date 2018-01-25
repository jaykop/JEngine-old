#version 410 core

////////////////////////////
// const variables
////////////////////////////
const int MAX_ARRAY			= 128;
const int LIGHT_DIRECTIONAL	= 1;
const int LIGHT_SPOTLIGHT	= 2;
const int LIGHT_POINTLIGHT	= 3;
const int EFFECT_NONE		= 0;
const int EFFECT_BLUR		= 1;
const int EFFECT_SOBEL		= 2;
const int EFFECT_INVERSE	= 3;

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
	
	bool m_offset;
};

////////////////////////////
// in variables
////////////////////////////
in	vec2 v2_outTexCoord;
in	vec3 v3_outNormal;
in 	vec3 v3_outFragmentPosition;

////////////////////////////
// out variables
////////////////////////////
out	vec4 v4_fragColor;

////////////////////////////
// uniform variables
////////////////////////////
uniform vec3 		v3_cameraPosition;
uniform vec4 		v4_color;
uniform vec4 		v4_lightColor[MAX_ARRAY];
uniform bool 		boolean_light;
uniform sampler2D 	Texture;
uniform Material 	material;
uniform Light		light[MAX_ARRAY];
uniform int			enum_effectType;
uniform float		float_blurSize;
uniform float		float_blurAmount;
uniform float		float_sobelAmount;
uniform int 		int_lightSize;

////////////////////////////
// function declarations
////////////////////////////
void LightingEffect(inout vec4 _light);
void VisualEffect(inout vec4 _color);

////////////////////////////
// entry point
////////////////////////////
void main() {

	vec4 finalTexture = vec4(0,0,0,0);
	

	// Any effect?
	if ((enum_effectType != EFFECT_NONE) 
		|| boolean_light) {
			
		// Implement light attributes
		if (boolean_light)
			LightingEffect(finalTexture);
				
		// Impose visual effect here...
		if (enum_effectType != EFFECT_NONE)
			VisualEffect(finalTexture);
	}
		
	// Unless..
	else
		finalTexture = texture(Texture, v2_outTexCoord)* v4_color;
		
	
	v4_fragColor = finalTexture;
}

////////////////////////////
// function bodies
////////////////////////////
void LightingEffect(inout vec4 _color) {

	// TODO
	// Dynamic light loop...
	for (int index = 0; index < 2; ++index) {
	
		vec3 	lightDirection;
		float 	attenuation = 1.f;
		vec3 	gap = light[index].m_position - v3_outFragmentPosition;
		float 	theta = 0.f;
		
		// Directional light
		if (light[index].m_type == LIGHT_DIRECTIONAL)
			lightDirection = normalize(-light[index].m_direction);
		else {
			lightDirection = normalize(gap);
		
			// Spotlight
			if (light[index].m_type == LIGHT_SPOTLIGHT) {
				theta = dot(lightDirection, normalize(-light[index].m_direction));
			}
			
			// Pointlight
			else if (light[index].m_type == LIGHT_POINTLIGHT) {
				float distance = length(gap);
				attenuation = 1.0 / (light[index].m_constant + light[index].m_linear * distance + light[index].m_quadratic * (distance * distance));
			}
		}	
		
		// Ambient light
		vec4 ambient = light[index].m_ambient * vec4(texture(material.m_diffuse, v2_outTexCoord)); 
		
		// Diffuse light
		vec3 norm = normalize(v3_outNormal);
		
		float diff = max(dot(norm, lightDirection), 0.0);
		vec4 diffuse = light[index].m_diffuse * vec4(diff * texture(material.m_diffuse, v2_outTexCoord)); 
		
		// Specular light
		vec3 viewDirection = normalize(v3_cameraPosition - v3_outFragmentPosition);
		vec3 reflectedDirection = reflect(-lightDirection, norm);
		float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.m_shininess);
		vec4 specular = light[index].m_specular * vec4(spec * texture(material.m_specular, v2_outTexCoord)); 
		
		// Smooth spotlight
		if (light[index].m_type == LIGHT_SPOTLIGHT) {
			float epsilon = light[index].m_cutOff - light[index].m_outerCutOff;
			float intensity = clamp((theta - light[index].m_outerCutOff) / epsilon, 0.0, 1.0);
			diffuse *= intensity;
			specular *= intensity;
		}
			
		// Add all light effects
		_color += v4_lightColor[index] * ((ambient + diffuse + specular) * attenuation);
	} 
	
	// Refresh alpha value
	_color.w = 1.0;
}


void VisualEffect(inout vec4 _color){

	// Blur effect
	if (enum_effectType == EFFECT_BLUR) {
		
		int x_range = int(float_blurSize / 2.0);
		int y_range = int(float_blurSize / 2.0);
		
		vec4 sum = vec4(0,0,0,0);
		for (int x = -x_range ; x <= x_range ; x++)
			for(int y = -y_range ; y <= y_range ; y++){
				sum += texture(Texture, 
					vec2(v2_outTexCoord.x + x * (1 / float_blurAmount), 
						v2_outTexCoord.y + y * (1 / float_blurAmount))) 
						/ (float_blurSize * float_blurSize);
			}
			
		_color = sum;
	}
	
	// Sobel effect
	else if (enum_effectType == EFFECT_SOBEL){
		vec4 top         = texture(Texture, vec2(v2_outTexCoord.x, v2_outTexCoord.y + 5.0 / float_sobelAmount));
		vec4 bottom      = texture(Texture, vec2(v2_outTexCoord.x, v2_outTexCoord.y - 5.0 / float_sobelAmount));
		vec4 left        = texture(Texture, vec2(v2_outTexCoord.x - 5.0 / float_sobelAmount, v2_outTexCoord.y));
		vec4 right       = texture(Texture, vec2(v2_outTexCoord.x + 5.0 / float_sobelAmount, v2_outTexCoord.y));
		vec4 topLeft     = texture(Texture, vec2(v2_outTexCoord.x - 5.0 / float_sobelAmount, v2_outTexCoord.y + 5.0 / float_sobelAmount));
		vec4 topRight    = texture(Texture, vec2(v2_outTexCoord.x + 5.0 / float_sobelAmount, v2_outTexCoord.y + 5.0 / float_sobelAmount));
		vec4 bottomLeft  = texture(Texture, vec2(v2_outTexCoord.x - 5.0 / float_sobelAmount, v2_outTexCoord.y - 5.0 / float_sobelAmount));
		vec4 bottomRight = texture(Texture, vec2(v2_outTexCoord.x + 5.0 / float_sobelAmount, v2_outTexCoord.y - 5.0 / float_sobelAmount));
		vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
		vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
		_color = sqrt(sx * sx + sy * sy);
	}
	
	// Inverse effect
	else if (enum_effectType == EFFECT_INVERSE)
	{
		vec4 inversed = vec4(1,1,1,1) - _color;
		inversed.w = 1.f;
		
		_color = inversed;
	}
}