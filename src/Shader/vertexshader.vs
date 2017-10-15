#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;

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

// uniform boolean
uniform bool boolean_flip;

// out variables
out		vec4 v4_outColor;
out		vec2 v2_outTexCoord;

void main(){

	// Calculate mvp transform matrix
	mat4 model =  m4_scale * m4_rotate * m4_translate;
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(model);

	v4_outColor = v4_color;
	gl_Position = mvp * vec4(position, 1);
	
	mat4 animation = m4_aniScale * m4_aniTranslate;
	vec4 newTexCoord = transpose(animation) * vec4(uvPosition, 0, 1);
	
	if (boolean_flip)
		newTexCoord.x = -newTexCoord.x;
		
	v2_outTexCoord = newTexCoord.xy;
}