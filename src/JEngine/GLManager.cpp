#include <cstdio>
#include <string>
#include "GLManager.h"
#include "Shader.hpp"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
float				GLManager::m_width = 0;
float				GLManager::m_height = 0;
GLint				GLManager::m_uniform[];
GLuint				GLManager::m_vao = 0;
GLuint				GLManager::m_vbo = 0;
GLuint				GLManager::m_ebo = 0;
GLuint				GLManager::m_lightVao = 0;
//GLuint				GLManager::m_depthBuf = 0;
//GLuint				GLManager::m_colorTex = 0;
//GLuint				GLManager::m_normalTex = 0;
//GLuint				GLManager::m_deferredFBO = 0;
//GLuint				GLManager::m_positionTex = 0;
GLuint				GLManager::m_passIndex[];
GLuint				GLManager::m_particleEbo = 0;
GLuint				GLManager::m_particleVao = 0;
GLuint				GLManager::m_particleVbo = 0;
GLuint				GLManager::m_particleColor = 0;
GLuint				GLManager::m_particlePosition = 0;
const int			GLManager::m_rect = 6;
const int			GLManager::m_cube = 36;
const int			GLManager::m_particle = 18;
//const unsigned		GLManager::m_glArrayMax = 128;
GLManager::Shaders	GLManager::m_shader;
GLManager::DrawMode GLManager::m_mode = DrawMode::DRAW_FILL;

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
bool GLManager::initSDL_GL(float _width, float _height)
{
	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	//Before using shader, initialize glew.
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "*GLManager: Failed to initialize GLEW\n");
		return false;
	}

	// Unless
	else {

		// Set wdith and heught
		m_width		= _width;
		m_height	= _height;

		// Do gl stuff
		ShowGLVersion();
		InitVBO();
		//InitFBO();
		InitGLEnvironment();
		InitShaders();
		RegisterUniform();

		//m_passIndex[0] = glGetSubroutineIndex(m_shader[SHADER_NORMAL]->m_programId, GL_FRAGMENT_SHADER, "pass1");
		//m_passIndex[1] = glGetSubroutineIndex(m_shader[SHADER_NORMAL]->m_programId, GL_FRAGMENT_SHADER, "pass2");

	}

	return true;
}

void GLManager::CloseSDL_GL()
{
	// Clear shaders
	for (auto shader : m_shader) {
		delete shader;
		shader = nullptr;
	}

	m_shader.clear();
}

void GLManager::InitVBO()
{
	glActiveTexture(GL_TEXTURE0);

	// Generate vertex array object(VAO)
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	// Generate vertex buffer object(VBO)
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

	// TODO
	// Particle
	// Generate vertex array object for particle(VAO)
	glGenVertexArrays(1, &m_particleVao);
	glBindVertexArray(m_particleVao);

	glGenBuffers(1, &m_particleVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_particleVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_verticesParticle), m_verticesParticle, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribDivisor(0, 1);

	// text coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glVertexAttribDivisor(1, 1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//glVertexAttribDivisor(2, 1);

	glGenBuffers(1, &m_particleEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_particleEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indicesParticle), m_indicesParticle, GL_STATIC_DRAW);
}

void GLManager::InitGLEnvironment()
{
	// Show how much attributes are available
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	JE_DEBUG_PRINT("*GLManager: Maximum nr of vertex attributes supported - %d\n", nrAttributes);

	// Set how to draw
	SetDrawMode(m_mode);

	// Active blend function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LESS);
	glEnable(GL_DEPTH_TEST);

	/*
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);*/
	// Texture attribute setting

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

void GLManager::InitShaders() 
{
	// Do shader stuff
	for (unsigned i = 0; i < SHADER_END; ++i) 
		m_shader.push_back(new Shader);

	m_shader[SHADER_LIGHTING]->LoadShader(
		"../src/shader/lighting.vs",
		"../src/shader/lighting.fs");

	m_shader[SHADER_NORMAL]->LoadShader(
		"../src/shader/normal.vs",
		"../src/shader/normal.fs");

	m_shader[SHADER_PARTICLE]->LoadShader(
		"../src/shader/particle.vs",
		"../src/shader/particle.fs");
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
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_TRANSLATE, "m4_translate");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_SCALE, "m4_scale");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_ROTATE, "m4_rotate");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_CAMERA, "m4_viewport");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_PROJECTION, "m4_projection");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_ANI_TRANSLATE, "m4_aniTranslate");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_ANI_SCALE, "m4_aniScale");

	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_COLOR, "v4_color");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_CAMERA_POSITION, "v3_cameraPosition");
	
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_FLIP, "boolean_flip");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_IS_LIGHT, "boolean_light");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_LIGHT_SIZE, "int_lightSize");

	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_AMBIENT, "material.m_ambient");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_DIFFUSE, "material.m_diffuse");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_SPECULAR, "material.m_specular");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_MATERIAL_SHININESS, "material.m_shininess");

	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_TYPE, "enum_effectType");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_BLUR_SIZE, "float_blurSize");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_BLUR_AMOUNT, "float_blurAmount");
	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_EFFECT_SOBEL, "float_sobelAmount");

	m_shader[SHADER_NORMAL]->ConnectUniform(UNIFORM_PARTICLE_HIDE, "boolean_hideParticle");

	/******************** Light shader ********************/
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_TRANSLATE, "m4_translate");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_SCALE, "m4_scale");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_ROTATE, "m4_rotate");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_CAMERA, "m4_viewport");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_PROJECTION, "m4_projection");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_COLOR, "v4_color");

	/******************** Particle shader ********************/
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_COLOR, "v4_color");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_TRANSLATE, "m4_translate");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_SCALE, "m4_scale");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_ROTATE, "m4_rotate");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_CAMERA, "m4_viewport");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_PROJECTION, "m4_projection");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_TIME, "float_time");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_LIFETIME, "float_lifeTime");

}

void GLManager::ShowGLVersion()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	JE_DEBUG_PRINT("*GLManager: GL Vendor - %s\n", vendor);
	JE_DEBUG_PRINT("*GLManager: GL Renderer - %s\n", renderer);
	JE_DEBUG_PRINT("*GLManager: GL Version - %s\n", version);
	JE_DEBUG_PRINT("*GLManager: GLSL Version - %s\n", glslVersion);
}

//void GLManager::CreateGBufferTex(GLenum _texUnit, GLenum _format, GLuint &_texid) {
//	glActiveTexture(_texUnit);
//	glGenTextures(1, &_texid);
//	glBindTexture(GL_TEXTURE_2D, _texid);
//
//	glTexStorage2D(GL_TEXTURE_2D, 1, _format, GLsizei(m_width), GLsizei(m_height));
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
//}

//void GLManager::InitFBO()
//{
//	// Create and bind the FBO
//	glGenFramebuffers(1, &m_deferredFBO);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);
//
//	// The depth buffer
//	glGenRenderbuffers(1, &m_depthBuf);
//	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuf);
//	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
//
//	// Create the textures for position, normal and color
//	CreateGBufferTex(GL_TEXTURE0, GL_RGB32F, m_positionTex);	// Position
//	CreateGBufferTex(GL_TEXTURE1, GL_RGB32F, m_normalTex);		// Normal
//	CreateGBufferTex(GL_TEXTURE2, GL_RGB8, m_colorTex);			// Color
//
//	// Attach the textures to the framebuffer
//	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuf);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionTex, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_normalTex, 0);
//	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_colorTex, 0);
//
//	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
//		GL_COLOR_ATTACHMENT2 };
//	glDrawBuffers(4, drawBuffers);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//}

JE_END

