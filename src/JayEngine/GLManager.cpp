#include <cstdio>
#include "Shader.hpp"
#include "GLManager.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
GLuint GLManager::m_vao = 0;
GLuint GLManager::m_vbo = 0;
GLuint GLManager::m_ebo = 0;
GLManager::DrawMode GLManager::m_mode = DrawMode::DRAW_FILL;
GLint GLManager::m_uniformType[UNIFORM_END];

//////////////////////////////////////////////////////////////////////////
// vertex variables
//////////////////////////////////////////////////////////////////////////
const static float s_vertices[] =
{
	// vertic position	// texture coordinate
	-.5f, .5f, 0.f,		0.f, 0.f,	// top left	
	.5f, .5f, 0.f,		1.f, 0.f,	// top right
	.5f, -.5f,	0.f,	1.f, 1.f,	// bottom right
	-.5f, -.5f, 0.f,	0.f, 1.f	// bottom left
};

const static int s_indices[] =
{
	/***************/
	/*  *   second */
	/*     *       */
	/*        *    */
	/*  first    * */
	/***************/

	0 ,2, 3,	// first triangle
	2, 0, 1		// second triangle
};

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
		SetVao();	// Generate vertex array object
		SetVbo();	// Generate vertex buffuer object
		SetVA();	// Set vertex attributes pointers
		SetEbo();	// Set indices

		// Do gl stuff
		InitGLEnvironment();

		// Do shader stuff
		ActivateShader(
			"../../src/Shader/vertexshader.vs",		// vertex Shader
			"../../src/Shader/fragmentshader.fs");	// fragment Shader
		RegisterUniform();
	}

	return true;
}

void GLManager::CloseSDL_GL()
{
}

void GLManager::InitGLEnvironment()
{
	// Show how much attributes are available
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	JE_DEBUG_PRINT("Maximum nr of vertex attributes supported: %d\n", nrAttributes);

	// Active blend function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Texture attribute setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

void GLManager::ActivateShader(const char * _vertexDir, const char * _fregmentDir)
{
	// Load shader and compile and link
	Shader::LoadShader(_vertexDir, _fregmentDir);
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
	// Matrix uniform
	m_uniformType[UNIFORM_TRANSLATE] = glGetUniformLocation(Shader::m_programId, "m4_translate");
	m_uniformType[UNIFORM_SCALE] = glGetUniformLocation(Shader::m_programId, "m4_scale");
	m_uniformType[UNIFORM_ROTATE] = glGetUniformLocation(Shader::m_programId, "m4_rotate");
	m_uniformType[UNIFORM_CAMERA] = glGetUniformLocation(Shader::m_programId, "m4_viewport");
	m_uniformType[UNIFORM_PROJECTION] = glGetUniformLocation(Shader::m_programId, "m4_projection");
	
	m_uniformType[UNIFORM_ANI_TRANSLATE] = glGetUniformLocation(Shader::m_programId, "m4_aniTranslate");
	m_uniformType[UNIFORM_ANI_SCALE] = glGetUniformLocation(Shader::m_programId, "m4_aniScale");
	
	// Vector uniform
	m_uniformType[UNIFORM_COLOR] = glGetUniformLocation(Shader::m_programId, "v4_color");

	// Boolean uniform
	m_uniformType[UNIFORM_FLIP] = glGetUniformLocation(Shader::m_programId, "boolean_flip");
	
}

GLint GLManager::GetUniform(UniformType _uniform)
{
	return m_uniformType[_uniform];
}

void GLManager::SetVao()
{
	// TODO
	//glActiveTexture(GL_TEXTURE0);

	// Generate vertex array object(VAO)
	glGenVertexArrays(1, &m_vao);

	// Bind Vertex Array Object
	glBindVertexArray(m_vao);
}

void GLManager::SetVbo()
{
	// Generate vertex buffer object(VBO)
	glGenBuffers(1, &m_vbo);

	// Set vbo active
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Now we can copy vertex data to this vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
}

void GLManager::SetVA()
{
	// Interpret vertex attributes data (s_vertices)
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// text coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// TODO
	// maybe more...
}

void GLManager::SetEbo()
{
	// Just like vbo...
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices), s_indices, GL_STATIC_DRAW);
}

NS_JE_END

