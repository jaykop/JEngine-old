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

	enum DrawMode {DRAW_DOT, DRAW_LINE, DRAW_FILL};
	enum UniformType {UNIFORM_COLOR, UNIFORM_END};

	static bool initSDL_GL();
	static void CloseSDL_GL();
	static void ActivateShader(const char* _vertexDir, const char* _fregmentDir);

	static void SetDrawMode(DrawMode _mode);
	static GLint GetUniform(UniformType _uniform);

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
	static void RegisterUniform();

	static GLuint m_vbo, m_vao, m_ebo;
	static DrawMode m_mode;
	static GLint m_uniformType[UNIFORM_END];

};

NS_JE_END
