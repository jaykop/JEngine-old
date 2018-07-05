#include "Shader.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// Fragment shaders
//////////////////////////////////////////////////////////////////////////
std::string Shader::fragmentShader_[] = {

	/*************** Model Shader **************/
	R"glsl(
	#version 450 core

	layout (location = 0) out vec4 v4_fragColor;

	////////////////////////////
	// const variables
	////////////////////////////
	const int MAX_ARRAY			= 16;
	const int LIGHT_DIRECTIONAL	= 1;
	const int LIGHT_SPOTLIGHT	= 2;
	const int LIGHT_POINTLIGHT	= 3;

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
	
		float mconstant;
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
	// uniform variables
	////////////////////////////
	uniform int 		int_lightSize;
	uniform vec3 		v3_cameraPosition;
	uniform vec4 		v4_color;
	uniform vec4 		v4_lightColor[MAX_ARRAY];
	uniform bool 		boolean_light;
	uniform Light		light[MAX_ARRAY];
	uniform sampler2D 	Texture;
	uniform Material 	material;

	////////////////////////////
	// function declarations
	////////////////////////////
	void LightingEffect(inout vec4 _light);

	////////////////////////////
	// entry point
	////////////////////////////
	void main() {

		vec4 finalTexture = vec4(0,0,0,0);

		// Implement light attributes	
		if (boolean_light)
			LightingEffect(finalTexture);
		
		// Unless..
		else
			finalTexture = texture(Texture, v2_outTexCoord)* v4_color;
		
       if (finalTexture.a <= 0.0)
			discard;

		v4_fragColor = finalTexture;
	}

	////////////////////////////
	// function bodies
	////////////////////////////
	void LightingEffect(inout vec4 _color) {

		for (int index = 0; index < int_lightSize ; index++) {
	
			float 	attenuation = 1.f;
			vec3 	gap = light[index].m_position - v3_outFragmentPosition;
			vec3 	lightDirection = normalize(gap);
			float 	theta = 0.f;
			float 	intensity = 1.f;
		
			// Directional light
			if (light[index].m_type == LIGHT_DIRECTIONAL)
				lightDirection = normalize(-light[index].m_direction);
		 
			// Spotlight
			else if (light[index].m_type == LIGHT_SPOTLIGHT) {
				theta = dot(lightDirection, normalize(-light[index].m_direction));
				float epsilon = light[index].m_cutOff - light[index].m_outerCutOff;
				intensity = clamp((theta - light[index].m_outerCutOff) / epsilon, 0.0, 1.0);
			}
			
			// Pointlight
			else if (light[index].m_type == LIGHT_POINTLIGHT) {
				float distance = length(gap);
				attenuation = 1.0 / (light[index].mconstant + light[index].m_linear * distance + light[index].m_quadratic * (distance * distance));
			}
		
			// Ambient light
			vec4 ambient = light[index].m_ambient * vec4(texture(Texture, v2_outTexCoord)); 
		
			// Diffuse light
			vec3 norm = normalize(v3_outNormal);
		
			float diff = max(dot(norm, lightDirection), 0.0);
			vec4 diffuse = light[index].m_diffuse * vec4(diff * texture(Texture, v2_outTexCoord))* intensity;
		
			// Specular light
			vec3 viewDirection = normalize(v3_cameraPosition - v3_outFragmentPosition);
			vec3 reflectedDirection = reflect(-lightDirection, norm);
			float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.m_shininess);
			vec4 specular = light[index].m_specular * vec4(spec * texture(Texture, v2_outTexCoord)) * intensity; 
			
			// Add all light effects
			_color += v4_lightColor[index] * ((ambient + diffuse + specular) * attenuation);
		} 
	
		// Refresh alpha value
		_color.w = 1.0;
	}
	)glsl",

	/*************** Text Shader **************/
	R"glsl(
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

                 vec4 finalTexture = texture(Texture, v2_outTexCoord).r * v4_color;
                 if (finalTexture.a <= 0.0)
                    discard;

		v4_fragColor = finalTexture;
	
	}
	)glsl",

	/*************** Lighting Shader **************/
	R"glsl(
	#version 450 core

	uniform vec4 v4_color;
	out	vec4 v4_fragColor;

	void main()
	{
                if (v4_color.a <= 0.0)
                    discard;
		
                v4_fragColor = v4_color;
	}
	)glsl",

	/*************** Particle Shader **************/
	R"glsl(
	#version 450 core

	// Interpolated values from the vertex shaders
	in vec2 v2_outTexCoord;
	in vec4 v4_outColor;

	// Ouput data
	out vec4 color;

	uniform sampler2D 	Texture;

	void main(){

                vec4 finalTexture = texture( Texture, v2_outTexCoord ) * v4_outColor;
                if (finalTexture.a <= 0.0)
                    discard;

		color = finalTexture;

	}
	)glsl",

	/*************** Screen Shader **************/
	R"glsl(
	#version 450 core

	layout (location = 0) out vec4 v4_fragColor;

	const int JE_EFFECT_NONE		= 0;
	const int JE_EFFECT_BLUR		= 1;
	const int JE_EFFECT_SOBEL		= 2;
	const int JE_EFFECT_INVERSE	= 3;

	in vec2 v2_outTexCoord;

	uniform int			enum_effectType;
	uniform float		float_blurSize;
	uniform float		float_blurAmount;
	uniform float		float_sobelAmount;
	uniform vec4 		v4_screenColor;
	uniform sampler2D 	Framebuffer;

	void VisualEffect(inout vec4 _color, inout vec2 _textureCoord);

	void main()
	{

		vec2 textureCoord = v2_outTexCoord;
		textureCoord.y = 1 - textureCoord.y;
		vec4 finalScreen = texture(Framebuffer, textureCoord) * v4_screenColor;
	
		// Impose visual effect here...
		if (enum_effectType != JE_EFFECT_NONE)
			VisualEffect(finalScreen, textureCoord);

		if (finalScreen.a <= 0.0)
                    discard;

		v4_fragColor = finalScreen;
	}

	void VisualEffect(inout vec4 _color, inout vec2 _textureCoord){

		// Blur effect
		if (enum_effectType == JE_EFFECT_BLUR) {
		
			int x_range = int(float_blurSize / 2.0);
			int y_range = int(float_blurSize / 2.0);
		
			vec4 sum = vec4(0,0,0,0);
			for (int x = -x_range ; x <= x_range ; x++)
				for(int y = -y_range ; y <= y_range ; y++){
					sum += texture(Framebuffer, 
						vec2(_textureCoord.x + x * (1 / float_blurAmount), 
							_textureCoord.y + y * (1 / float_blurAmount))) 
							/ (float_blurSize * float_blurSize);
				}
			
			_color = sum;
		}
	
		// Sobel effect
		else if (enum_effectType == JE_EFFECT_SOBEL){
			vec4 top         = texture(Framebuffer, vec2(_textureCoord.x, _textureCoord.y + float_sobelAmount));
			vec4 bottom      = texture(Framebuffer, vec2(_textureCoord.x, _textureCoord.y - float_sobelAmount));
			vec4 left        = texture(Framebuffer, vec2(_textureCoord.x - float_sobelAmount, _textureCoord.y));
			vec4 right       = texture(Framebuffer, vec2(_textureCoord.x + float_sobelAmount, _textureCoord.y));
			vec4 topLeft     = texture(Framebuffer, vec2(_textureCoord.x - float_sobelAmount, _textureCoord.y + float_sobelAmount));
			vec4 topRight    = texture(Framebuffer, vec2(_textureCoord.x + float_sobelAmount, _textureCoord.y + float_sobelAmount));
			vec4 bottomLeft  = texture(Framebuffer, vec2(_textureCoord.x - float_sobelAmount, _textureCoord.y - float_sobelAmount));
			vec4 bottomRight = texture(Framebuffer, vec2(_textureCoord.x + float_sobelAmount, _textureCoord.y - float_sobelAmount));
			vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
			vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
			_color = sqrt(sx * sx + sy * sy);
		}
	
		// Inverse effect
		else if (enum_effectType == JE_EFFECT_INVERSE)
		{
			vec4 inversed = vec4(1,1,1,1) - _color;
			inversed.w = 1.f;
		
			_color = inversed;
		}
	}
	)glsl",
};

jeEnd
