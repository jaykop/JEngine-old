#version 330 core
layout (location = 0) in vec3 position;

// matrix uniform
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;

// vector uniform
uniform vec4 v4_color;
out		vec4 v4_outColor;

void main(){

	// Calculate mvp transform matrix
	mat4 model =  m4_scale * m4_rotate * m4_translate;
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(model);

	v4_outColor = v4_color;
	gl_Position = mvp * vec4(position, 1);
}