#include <cstdio>
#include <string>
#include "GLManager.h"
#include "Shader.hpp"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
GLuint GLManager::m_vao = 0;
GLuint GLManager::m_vbo = 0;
GLuint GLManager::m_ebo = 0;
GLint GLManager::m_uniform[];
GLManager::Shaders GLManager::m_shaders;
GLManager::DrawMode GLManager::m_mode = DrawMode::DRAW_FILL;
unsigned GLManager::m_glArraySize = 128;
const int GLManager::m_cube = 36;
const int GLManager::m_rect = 6;
const int GLManager::m_particle = 18;

const float GLManager::m_vertices2d[] = {
	// position				// uv		// normals
	-.5f,	.5f,	0.f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	-.5f,	0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	-.5f,	0.f,	0.f, 0.f,	0.0f,  0.0f, 1.0f		// bottom left
};

const unsigned GLManager::m_indices2d[] = {
	// front
	0, 2, 3,	// first triangle
	2, 0, 1		// second triangle
};

const float GLManager::m_verticesParticle[] = {

	// position				// uv		// normals
	-.5f,	.5f,	0.f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	-.5f,	0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	-.5f,	0.f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,		// bottom left

	// position				// uv		// normals
	0.f,	.5f,	.5f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	0.f,	.5f,	-.5f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	0.f,	-.5f,	-.5f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	0.f,	-.5f,	.5f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,		// bottom left

	// position				// uv		// normals
	-.5f,	0.f,	-.5f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	0.f,	-.5f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	0.f,	.5f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	0.f,	.5f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,		// bottom left

};
const unsigned GLManager::m_indicesParticle[] = {
	// front
	0, 2, 3,	// first triangle
	2, 0, 1,	// second triangle

	// back
	5, 7, 6,	// first triangle
	7, 5, 4,	// second triangle

	// left
	8, 10, 11,	// first triangle
	10, 8, 9	// second triangle
};

const float GLManager::m_vertices [] = 
{
	// front
	// position				// uv		// normals
	-.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	.5f,	.5f,	.5f, .25f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	-.5f,	.5f,	.5f, .5f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	-.5f,	.5f,	.25f, .5f,	0.0f,  0.0f, 1.0f,		// bottom left

	// back
	// position				// uv		// normals
	.5f,	.5f,	-.5f,	.75f, .25f,	0.0f,  0.0f, -1.0f,		// top left	
	-.5f,	.5f,	-.5f,	1.f, .25f,	0.0f,  0.0f, -1.0f,		// top right
	-.5f,	-.5f,	-.5f,	1.f, .5f,	0.0f,  0.0f, -1.0f,		// bottom right
	.5f,	-.5f,	-.5f,	.75f, .5f,	0.0f,  0.0f, -1.0f,		// bottom left

	// left
	// position				// uv		// normals
	-.5f,	.5f,	-.5f,	0.f, .25f,	-1.0f,  0.0f,  0.0f,	// top left	
	-.5f,	.5f,	.5f,	.25f, .25f,	-1.0f,  0.0f,  0.0f,	// top right
	-.5f,	-.5f,	.5f,	.25f, .5f,	-1.0f,  0.0f,  0.0f,	// bottom right
	-.5f,	-.5f,	-.5f,	0.f, .5f,	-1.0f,  0.0f,  0.0f,	// bottom left

	// right
	// position				// uv		// normals
	.5f,	.5f,	.5f,	.5f, .25f,	1.0f,  0.0f,  0.0f,		// top left	
	.5f,	.5f,	-.5f,	.75f, .25f,	1.0f,  0.0f,  0.0f,		// top right
	.5f,	-.5f,	-.5f,	.75f, .5f,	1.0f,  0.0f,  0.0f,		// bottom right
	.5f,	-.5f,	.5f,	.5f, .5f,	1.0f,  0.0f,  0.0f,		// bottom left

	// down
	// position				// uv		// normals
	-.5f,	-.5f,	.5f,	.25f, .5f,	0.0f, -1.0f,  0.0f,		// top left	
	.5f,	-.5f,	.5f,	.5f, .5f,	0.0f, -1.0f,  0.0f,		// top right
	.5f,	-.5f,	-.5f,	.5f, .75f,	0.0f, -1.0f,  0.0f,		// bottom right
	-.5f,	-.5f,	-.5f,	.25f, .75f,	0.0f, -1.0f,  0.0f,		// bottom left

	// up
	// position				// uv		// normals
	-.5f,	.5f,	-.5f,	.25f, 0.f,	0.0f,  1.0f,  0.0f,		// top left	
	.5f,	.5f,	-.5f,	.5f, 0.f,	0.0f,  1.0f,  0.0f,		// top right
	.5f,	.5f,	.5f,	.5f, .25f,	0.0f,  1.0f,  0.0f,		// bottom right
	-.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  1.0f,  0.0f		// bottom left
};

const unsigned GLManager::m_indices [] =
{
	//				 4					5
	//				  ****************
	//			  *				 *	 */
	//		0 *				 *		 */
	//		/***************/ 1		 */
	//		/*  *   second */		 */
	//		/*     *       */		 */
	//		/*        *    */		 */ 6
	//		/*  first    * */	 *
	//		/***************/*
	//		3				2

	// front
	0, 2, 3,	// first triangle
	2, 0, 1,	// second triangle

	// back
	5, 7, 6,	// first triangle
	7, 5, 4,	// second triangle

	// left
	8, 10, 11,	// first triangle
	10, 8, 9,	// second triangle

	// right
	12, 14, 15,	// first triangle
	14, 12, 13,	// second triangle

	// down
	16, 18, 19,	// first triangle
	18, 16, 17,	// second triangle

	// up
	20 ,22, 23,	// first triangle
	22, 20, 21	// second triangle
};

//////////////////////////////////////////////////////////////////////////
// GLManager functio bodies
//////////////////////////////////////////////////////////////////////////
bool GLManager::initSDL_GL()
{
	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	//Before using shader, initialize glew.
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	// Unless
	else {

		// Do gl stuff
		InitGLEnvironment();
		InitShaders();
		RegisterUniform();
	}

	return true;
}

void GLManager::CloseSDL_GL()
{
	// Clear shaders
	for (auto shader : m_shaders) {
		delete shader;
		shader = nullptr;
	}

	m_shaders.clear();
}

void GLManager::InitGLEnvironment()
{
	glActiveTexture(GL_TEXTURE0);

	// Generate vertex array object(VAO)
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate vertex buffer object(VBO)
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Interpret vertex attributes data (s_vertices)
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// text coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// normals of vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	// Show how much attributes are available
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	JE_DEBUG_PRINT("Maximum nr of vertex attributes supported: %d\n", nrAttributes);

	// Set how to draw
	SetDrawMode(m_mode);

	// Active blend function
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	// Texture attribute setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

void GLManager::InitShaders() 
{
	// Do shader stuff
	for (unsigned i = 0; i < SHADER_END; ++i) 
		m_shaders.push_back(new Shader);

	m_shaders[SHADER_LIGHTING]->LoadShader(
		"../src/shader/lighting.vs",
		"../src/shader/lighting.fs");

	m_shaders[SHADER_NORMAL]->LoadShader(
		"../src/shader/normal.vs",
		"../src/shader/normal.fs");
}

void GLManager::SetDrawMode(DrawMode _mode)
{
	switch (_mode)
	{
	case DRAW_DOT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case DRAW_LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case DRAW_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	m_mode = _mode;
}

void GLManager::RegisterUniform()
{
	/******************** normal shader ********************/
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_TRANSLATE, "m4_translate");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_SCALE, "m4_scale");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_ROTATE, "m4_rotate");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_CAMERA, "m4_viewport");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_PROJECTION, "m4_projection");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_ANI_TRANSLATE, "m4_aniTranslate");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_ANI_SCALE, "m4_aniScale");

	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_COLOR, "v4_color");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_CAMERA_POSITION, "v3_cameraPosition");
	
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_FLIP, "boolean_flip");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_IS_LIGHT, "boolean_light");

	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_AMBIENT, "material.m_ambient");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_DIFFUSE, "material.m_diffuse");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_SPECULAR, "material.m_specular");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_SHININESS, "material.m_shininess");

	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_TYPE, "enum_effectType");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_BLUR_SIZE, "float_blurSize");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_BLUR_AMOUNT, "float_blurAmount");
	m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_SOBEL, "float_sobelAmount");

	for (unsigned i = 0; i < m_glArraySize; ++i) {

		std::string index = std::to_string(i);

		std::string color = "v4_lightColor[" + index + "]";
		std::string light = "light[" + index + "].";

		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_COLOR, color.c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_TYPE, light.append("m_type").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_DIFFUSE, light.append("m_diffuse").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_SPECULAR, light.append("m_specular").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_POSITION, light.append("m_position").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_DIRECTION, light.append("m_direction").c_str());

		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_CONST, light.append("m_constant").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_LINEAR, light.append("m_linear").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_QUAD, light.append("m_quadratic").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_CUTOFF, light.append("m_cutOff").c_str());
		m_shaders[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_OUTERCUTOFF, light.append("m_outerCutOff").c_str());
	}

	/******************** Light shader ********************/
	m_shaders[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_TRANSLATE, "m4_translate");
	m_shaders[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_SCALE, "m4_scale");
	m_shaders[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_ROTATE, "m4_rotate");
	m_shaders[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_CAMERA, "m4_viewport");
	m_shaders[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_PROJECTION, "m4_projection");
}

JE_END

