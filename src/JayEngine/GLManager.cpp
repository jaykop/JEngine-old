#include <cstdio>
#include "Shader.hpp"
#include "GLManager.h"

NS_JE_BEGIN

GLuint GLManager::m_vao = 0;
GLuint GLManager::m_vbo = 0;
GLuint GLManager::m_ebo = 0;
GLManager::DrawMode GLManager::m_mode = DrawMode::GRAPHIC_FILL;
GLint GLManager::m_uniformType[GRAPHIC_END];

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

		// Show how much attributes are available
		int nrAttributes;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
		JE_DEBUG_PRINT("Maximum nr of vertex attributes supported: %d\n", nrAttributes);
	}

	return true;
}

void GLManager::CloseSDL_GL()
{
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
	case GRAPHIC_DOT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case GRAPHIC_LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case GRAPHIC_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	m_mode = _mode;
}

void GLManager::RegisterUniform()
{

}

void GLManager::SetVao()
{
	// TODO
	//glActiveTexture(GL_TEXTURE0);

	// Generate vertex array object(VAO)
	glGenVertexArrays(1, &m_vao);

	// 1. bind Vertex Array Object
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

