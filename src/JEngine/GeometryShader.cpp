#include "Shader.h"

JE_BEGIN

std::string  Shader::m_geometryShader[] = {

	/*************** Model Shader **************/
	R"glsl(
	#version 450 core
	
	layout (triangles) in;
	layout(triangle_strip, max_vertices = 3) out;
	
	void main() {
	
	gl_Position = gl_in[0].gl_Position + vec4(-1.0, 0.0, 0.0, 0.0);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position + vec4(1.0, 0.0, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position + vec4(-1.0, 0.0, 0.0, 0.0);
	EmitVertex();
	
	gl_Position = gl_in[1].gl_Position + vec4(1.0, 0.0, 0.0, 0.0);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position + vec4(-1.0, 0.0, 0.0, 0.0);
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position + vec4(1.0, 0.0, 0.0, 0.0);
	EmitVertex();
	
	EndPrimitive();

	}
	)glsl",

	/*************** Text Shader **************/
	R"glsl(
	#version 450 core

	void main() {
	}
	)glsl",

	/*************** Lighting Shader **************/
	R"glsl(
	#version 450 core

	void main()
	{
	}
	)glsl",
	/*************** Lighting Shader **************/

	/*************** Particle Shader **************/
	R"glsl(
	#version 450 core

	void main(){
	}
	)glsl",

	/*************** Screen Shader **************/
	R"glsl(
	#version 450 core

	void main()
	{
	}
	)glsl",
};

JE_END