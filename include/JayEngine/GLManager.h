#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
NS_JE_BEGIN

// Vertex data
const static float s_vertices[] = 
{
	-.5f, .5f, 0.f,		// top left
	.5f, .5f, 0.f,		// top right
	.5f, -.5f,	0.f,	// bottom right
	-.5f, -.5f, 0.f		// bottom left
};

// Index data
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

class GLManager {

public:

	enum DrawMode {GRAPHIC_DOT, GRAPHIC_LINE, GRAPHIC_FILL};

	enum UniformType {GRAPHIC_COLOR, GRAPHIC_END};

	static bool initSDL_GL();
	static void CloseSDL_GL();
	static void ActivateShader(const char* _vertexDir, const char* _fregmentDir);

	static void SetDrawMode(DrawMode _mode);
	static void RegisterUniform();

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
	static void SetEbo();

	static GLuint m_vbo, m_vao, m_ebo;
	static DrawMode m_mode;
	static GLint m_uniformType[GRAPHIC_END];

};

NS_JE_END
