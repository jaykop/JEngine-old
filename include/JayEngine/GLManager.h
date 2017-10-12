#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
NS_JE_BEGIN

// Vertex data
const static float s_vertices[] = 
{
	// vertic position	// texture coordinate
	-.5f, .5f, 0.f,		0.f, 0.f,	// top left	
	.5f, .5f, 0.f,		1.f, 0.f,	// top right
	.5f, -.5f,	0.f,	1.f, 1.f,	// bottom right
	-.5f, -.5f, 0.f,	0.f, 1.f	// bottom left
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

	friend class Application;
	friend class GraphicSystem;
	enum DrawMode { DRAW_DOT, DRAW_LINE, DRAW_FILL };
	enum UniformType {

		// Martix uniform
		UNIFORM_TRANSLATE, UNIFORM_SCALE, UNIFORM_ROTATE,
		UNIFORM_CAMERA, UNIFORM_PROJECTION, 
		
		UNIFORM_ANI_TRANSLATE, UNIFORM_ANI_SCALE,

		// Vector uniform
		UNIFORM_COLOR, 

		// Last enum
		UNIFORM_END
	};

public:

	static void SetDrawMode(DrawMode _mode);

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

	static bool		initSDL_GL();
	static void		CloseSDL_GL();
	static void		RegisterUniform();
	static void		InitGLEnvironment();
	static void		ActivateShader(const char* _vertexDir, const char* _fregmentDir);
	static GLint	GetUniform(UniformType _uniform);

	// Private member variables
	static DrawMode m_mode;
	static GLuint	m_vbo, m_vao, m_ebo;
	static GLint	m_uniformType[UNIFORM_END];

};

NS_JE_END
