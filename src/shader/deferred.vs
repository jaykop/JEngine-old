#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec3 v3_outPosition;
out vec2 v2_outTexCoord;
out vec3 v3_outNormal;

uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
//uniform mat3 m3_normal;

void main(){

	mat4 model = m4_scale * m4_rotate * m4_translate;
	mat4 modelView = transpose(m4_viewport) * transpose(model);
	mat3 m3_normal = mat3(modelView);
	v3_outNormal = normalize(m3_normal * normal);
	v3_outPosition = vec3(modelView * vec4(position, 1));
	v2_outTexCoord = texCoord;
	gl_Position = transpose(m4_projection) * modelView * vec4(position, 1);

}