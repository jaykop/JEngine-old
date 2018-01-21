#version 410 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 offset;
layout (location = 4) in vec4 color;


// uniform matrixs
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform vec4 v4_color;

uniform float float_time;
uniform float float_lifeTime;

out	vec2 v2_outTexCoord;
out vec4 v4_outColor;

void main(){

	mat4 model = m4_scale * m4_rotate * m4_translate;
	vec4 newPos = transpose(model) * vec4(position, 1); 
	//mat4 mvp =  *
	
	gl_Position = transpose(m4_projection) * transpose(m4_viewport) *  vec4(newPos.xyz + offset, 1);
	v2_outTexCoord = uvPosition;
	v4_outColor = color;
}

