#pragma once

#include "GL/glew.h"
#define GLEW_STATIC

#include "Macro.h"
#include "Shader.hpp"

NS_JE_BEGIN

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

		// Boolean uniform
		UNIFORM_FLIP,

		// Lighting shader
		// Martix uniform
		UNIFORM_TRANSLATE_LIGHT, UNIFORM_SCALE_LIGHT, UNIFORM_ROTATE_LIGHT,
		UNIFORM_CAMERA_LIGHT, UNIFORM_PROJECTION_LIGHT,

		// Vector uniform	// TODO : Delete this
		UNIFORM_COLOR_LIGHT, UNIFORM_COLOR_OBJECT,

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

	// Private member variables
	static DrawMode m_mode;
	static GLuint	m_vbo, m_vao, m_ebo, m_light_vao;
	static Shader	m_shader[SHADER_END];
	static GLint	m_uniform[UNIFORM_END];

	static const float	s_vertices[20];
	static const float	s_vertices3D[40]; 

	static const int	s_indices[6];
	static const int	s_indices3D[36];
};

NS_JE_END
