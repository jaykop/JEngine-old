#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
NS_JE_BEGIN

// Vertex data
const static float s_vertices[] = 
{
	0.f, .5f,		//vertex 1
	5.f, -.5f		//vertex 2
	- .5f, -.5f,	//vertex 3	
};

class GLManager {

public:

	static bool initSDL_GL();
	static void CloseSDL_GL();

private:

	// Locked functions
	GLManager() {};
	~GLManager() {};
	GLManager(const GLManager& /*app*/) {};
	void operator=(const GLManager& /*app*/) {};

	// Private member functions
	void SetVbo();

	GLuint m_vbo;

};

NS_JE_END
