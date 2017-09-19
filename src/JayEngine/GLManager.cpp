#include "GLManager.h"

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

NS_JE_END

