#include "Shader.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// Vectex shaders
//////////////////////////////////////////////////////////////////////////
std::string Shader::m_vertexShader[] = {

    /*************** Model Shader **************/
    R"glsl(
	#version 450 core

	layout(location = 0) in vec3 position;
	layout(location = 1) in vec2 uvPosition;
	layout(location = 2) in vec3 normal;

	////////////////////////////
	// const variables
	////////////////////////////
	const int MAX_ARRAY = 16;

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
	uniform bool boolean_flip;
	uniform bool boolean_light;
	uniform bool boolean_bilboard;
	
	uniform bool hasParent;
	uniform mat4 m4_parentTranslate,
		m4_parentScale, m4_parentRotate;

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
	void main() {

		vec4 newPosition = vec4(position, 1);
		mat4 model = m4_scale * m4_rotate * m4_translate;
		vec4 newTexCoord;

		Transforming(newPosition, model);
		Mapping(newPosition, newTexCoord);
		SendToFrag(newPosition, newTexCoord, model);

	}

	////////////////////////////
	// Fucntion bodies
	////////////////////////////
	void Transforming(vec4 _position, mat4 _model) {
		
		mat4 newModel = transpose(_model);
		if (hasParent) 
			newModel = transpose(m4_parentScale * m4_parentRotate * m4_parentTranslate) * newModel;
	
		// Calculate mvp transform matrix
		mat4 modelview = transpose(m4_viewport) * newModel;

		if (boolean_bilboard) {
			modelview[0][0]
				= modelview[1][1]
				= modelview[2][2] = 1;

			modelview[0][1]
				= modelview[0][2]
				= modelview[1][0]
				= modelview[1][2]
				= modelview[2][0]
				= modelview[2][1] = 0;
		}

		mat4 mvp = transpose(m4_projection) * modelview;
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
	};
	)glsl",

    /*************** Text Shader **************/
    R"glsl(
	#version 450 core

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
	uniform bool boolean_bilboard;

	////////////////////////////
	// out variables
	////////////////////////////
	out	vec2 v2_outTexCoord;

	////////////////////////////
	// entry point
	////////////////////////////
	void main(){

		vec4 newPosition = vec4(position, 1);
		mat4 model =  m4_scale * m4_rotate * m4_translate;
	
		// Calculate mvp transform matrix
		mat4 modelview = transpose(m4_viewport) * transpose(model);
		
			if (boolean_bilboard) {
				modelview[0][0] 
				= modelview[1][1] 
				= modelview[2][2] = 1;
			
				modelview[0][1]
				= modelview[0][2]
				= modelview[1][0]
				= modelview[1][2]
				= modelview[2][0]
				= modelview[2][1] = 0;
			}
		
		mat4 mvp = transpose(m4_projection) * modelview;
		gl_Position = mvp * newPosition;
	
		// Texture coordinate
		v2_outTexCoord = uvPosition;
	}
	)glsl",

    /*************** Lighting Shader **************/
    R"glsl(
	#version 450 core

	layout (location = 0) in vec3 position;
	layout (location = 1) in vec2 uvPosition;
	layout (location = 2) in vec3 normal;

	// uniform matrixs
	uniform mat4 m4_translate;
	uniform mat4 m4_scale;
	uniform mat4 m4_viewport;
	uniform mat4 m4_projection;
	uniform mat4 m4_rotateY;
	uniform mat4 m4_rotateZ;

	void main(){

		mat4 model =  m4_scale * (m4_rotateY * m4_rotateZ) * m4_translate;
		mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(model);

		gl_Position = mvp * vec4(position, 1);
	}
	)glsl",

    /*************** Particle Shader **************/
    R"glsl(
	#version 450 core

	// Input vertex data, different for all executions of this shader.
	layout (location = 0) in vec3 position;
	layout (location = 1) in vec2 uvPosition;
	layout (location = 2) in vec3 normal;

	// uniform matrixs
	uniform mat4 m4_translate;
	uniform mat4 m4_scale;
	uniform mat4 m4_rotate;
	uniform mat4 m4_viewport;
	uniform mat4 m4_projection;
	uniform vec4 v4_color;

	uniform bool boolean_hide;
	uniform bool boolean_bilboard;

	out	vec2 	v2_outTexCoord;
	out vec4 	v4_outColor;

	void main(){

		if (!boolean_hide) {
	
			mat4 model = m4_scale * m4_rotate * m4_translate;
			mat4 modelview = transpose(m4_viewport) * transpose(model);
		
			if (boolean_bilboard) {
				modelview[0][0] 
				= modelview[1][1] 
				= modelview[2][2] = 1;
			
				modelview[0][1]
				= modelview[0][2]
				= modelview[1][0]
				= modelview[1][2]
				= modelview[2][0]
				= modelview[2][1] = 0;
			}
		
			mat4 mvp = transpose(m4_projection) * modelview;

			gl_Position = mvp *  vec4(position, 1); 
			v2_outTexCoord = uvPosition;
			v4_outColor = v4_color;
		}
	
		else {
			gl_Position = vec4(0,0,0,0); 
			v2_outTexCoord = vec2(0,0);
			v4_outColor = vec4(0,0,0,0);	
		}
	}
	)glsl",

    /*************** Screen Shader **************/
    R"glsl(
	#version 450 core

	layout (location = 0) in vec3 position;
	layout (location = 1) in vec2 uvPosition;
	layout (location = 2) in vec3 normal;

	out vec2 v2_outTexCoord;

	void main(){

		vec4 newPosition = vec4(2 * position, 1);
		v2_outTexCoord = uvPosition;
		gl_Position = newPosition;
	
	}
	)glsl",
};

jeEnd
