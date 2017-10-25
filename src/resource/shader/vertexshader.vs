#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;

// uniform matrixs
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;

uniform mat4 m4_aniScale;
uniform mat4 m4_aniTranslate;
uniform mat4 m4_animation;

// uniform vectors
uniform vec4 v4_color;
uniform vec4 v4_lightColor;
uniform vec3 v3_lightPosition;
uniform vec3 v3_cameraPosition;

// uniform boolean
uniform bool boolean_flip;
uniform bool boolean_light;

// out variables
out	vec4 v4_outColor;
out	vec2 v2_outTexCoord;
out	vec3 v3_outNormal;
out	vec4 v4_outLightColor;
out	vec3 v3_outLightPosition;
out	vec3 v3_outFragmentPosition;
out	vec3 v3_outCameraPosition;

void main(){

	// Calculate mvp transform matrix
	mat4 model =  m4_scale * m4_rotate * m4_translate;
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(model);
	
	vec4 newPosition = vec4(position, 1);
	gl_Position = mvp * newPosition;
	
	// Color mapping
	v4_outColor = v4_color;
	
	if (boolean_light) {
		// Lighing attributes
		v3_outFragmentPosition = vec3(model * newPosition);
	
		v4_outLightColor = v4_lightColor;
		v3_outLightPosition = v3_lightPosition;
		v3_outNormal = mat3(transpose(inverse(model))) * normal;
	
		v3_outCameraPosition = v3_cameraPosition;
	}
	
	// Texture mapping
	mat4 animation = m4_aniScale * m4_aniTranslate;
	vec4 newTexCoord = transpose(animation) * vec4(uvPosition, 0, 1);
	
	if (boolean_flip)
		newTexCoord.x = -newTexCoord.x;
		
	v2_outTexCoord = newTexCoord.xy;
}