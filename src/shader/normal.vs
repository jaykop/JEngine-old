#version 410 core

#define MAX_ARRAY 128

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;

////////////////////////////
// uniform variables
////////////////////////////
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;

uniform mat4 m4_aniScale;
uniform mat4 m4_aniTranslate;

uniform vec4 v4_lightColor[MAX_ARRAY];

uniform bool	boolean_flip;
uniform bool 	boolean_light;

////////////////////////////
// out variables
////////////////////////////
out	vec2 v2_outTexCoord;
out	vec3 v3_outNormal;
out	vec3 v3_outFragmentPosition;

////////////////////////////
// fucntion declarations
////////////////////////////
void Transforming(vec4 _position, mat4 _model);
void Mapping(vec4 _position, inout vec4 _texCoord);
void SendToFrag(vec4 _position, vec4 _texCoord, mat4 _model);

////////////////////////////
// entry point
////////////////////////////
void main(){

	vec4 newPosition = vec4(position, 1);
	mat4 model =  m4_scale * m4_rotate * m4_translate;
	vec4 newTexCoord;
	
	Transforming(newPosition, model);
	Mapping(newPosition, newTexCoord);
	SendToFrag(newPosition, newTexCoord, model);
	
}

////////////////////////////
// Fucntion bodies
////////////////////////////
void Transforming(vec4 _position, mat4 _model) {

	// Calculate mvp transform matrix
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(_model);
	
	gl_Position = mvp * _position;
}

void Mapping(vec4 _position, inout vec4 _texCoord) {
	
	// Animation mapping
	mat4 animation = m4_aniScale * m4_aniTranslate;
	_texCoord = transpose(animation) * vec4(uvPosition, 0, 1);

	// Check flipping
	if (boolean_flip)
		_texCoord.x = -_texCoord.x;
		
}

void SendToFrag(vec4 _position, vec4 _texCoord, mat4 _model) {
	
	// Texture coordinate
	v2_outTexCoord = _texCoord.xy;
	
	// Lighting attributes
	if (boolean_light) {
	
		// Lighing attributes
		v3_outFragmentPosition = vec3(transpose(_model) * _position);
	
		v3_outNormal = mat3(inverse(_model)) * normal;
	}
}