#include "GLManager.h"
#include <cstdio>

NS_JE_BEGIN

bool GLManager::initSDL_GL()
{
	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	//Before using shader, initialize glew.
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	//GLuint vertexBuffer;
	//glGenBuffers(1, &vertexBuffer);
	//JE_DEBUG_PRINT("%u\n", vertexBuffer);

	return true;
}

void GLManager::CloseSDL_GL()
{
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

NS_JE_END

