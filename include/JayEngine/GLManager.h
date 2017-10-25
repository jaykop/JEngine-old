#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
#include "Shader.hpp"

JE_BEGIN

class GLManager {

	friend class Application;
	friend class GraphicSystem;

	enum DrawMode { DRAW_DOT, DRAW_LINE, DRAW_FILL };
	enum ShaderType {SHADER_NORMAL, SHADER_LIGHTING, SHADER_END};
	enum UniformType {

		// Normal shader
		// Martix uniform
		UNIFORM_TRANSLATE, UNIFORM_SCALE, UNIFORM_ROTATE,
		UNIFORM_CAMERA, UNIFORM_PROJECTION,

		UNIFORM_ANI_TRANSLATE, UNIFORM_ANI_SCALE,

		// Vector uniform
		UNIFORM_COLOR,
		UNIFORM_LIGHT_COLOR, UNIFORM_LIGHT_POSITION,
		UNIFORM_CAMERA_POSITION,

		// Boolean uniform
		UNIFORM_FLIP, UNIFORM_IS_LIGHT,

		// Float uniform
		UNIFORM_LIGHT_AMBIENT, UNIFORM_LIGHT_SPECULAR,

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

	static bool	initSDL_GL();
	static void	CloseSDL_GL();
	static void	RegisterUniform();
	static void	InitGLEnvironment();

	// Private member variables
	static DrawMode m_mode;
	static GLuint	m_vao, m_vbo, m_vbo3D, m_ebo, m_ebo3D, m_light_vao;
	static Shader	m_shader[SHADER_END];
	static GLint	m_uniform[UNIFORM_END];

	static const float	m_vertices[32];
	static const int	m_indices[6]; 
	
#ifdef JE_SUPPORT_3D
	static const float	m_vertices3D[192];
	static const int	m_indices3D[36];
#endif

};

JE_END
