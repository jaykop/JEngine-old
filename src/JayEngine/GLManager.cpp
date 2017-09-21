#include <cstdio>
#include "Shader.hpp"
#include "GLManager.h"

NS_JE_BEGIN

GLuint GLManager::m_vao = 0;
GLuint GLManager::m_vbo = 0;

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

NS_JE_END

