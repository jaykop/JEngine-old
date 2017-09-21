#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
NS_JE_BEGIN

// Vertex data
const static float s_vertices[] = 
{
	-.5f, -.5f,	0.f,	//vertex 1
	.5f, -.5f, 0.f,		//vertex 2
	0.f, .5f, 0.f		//vertex 3
	
};

class GLManager {

public:

	static bool initSDL_GL();
	static void CloseSDL_GL();
	static void ActivateShader(const char* _vertexDir, const char* _fregmentDir);

private:

	// Locked functions
	GLManager() {};
	~GLManager() {};
	GLManager(const GLManager& /*app*/) {};
	void operator=(const GLManager& /*app*/) {};

	// Private member functions
	static void SetVbo();
	static void SetVao();
	static void SetVA();

	static GLuint m_vbo, m_vao;

};

NS_JE_END
