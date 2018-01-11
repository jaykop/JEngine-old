#pragma once

#include "glew.h"
#define GLEW_STATIC

#include <vector>
#include "Macro.h"

JE_BEGIN

class GLManager {

	friend class Shader;
	friend class Application;
	friend class GraphicSystem;

	using Shaders = std::vector<Shader*>;

	enum DrawMode { DRAW_DOT, DRAW_LINE, DRAW_FILL };
	enum ShaderType {SHADER_NORMAL, SHADER_LIGHTING, SHADER_END};
	enum UniformType {

		/******************** Normal shader ********************/
		//////////////////////
		// Martix uniform
		//////////////////////
		UNIFORM_TRANSLATE, UNIFORM_SCALE, UNIFORM_ROTATE,
		UNIFORM_CAMERA, UNIFORM_PROJECTION,
		UNIFORM_ANI_TRANSLATE, UNIFORM_ANI_SCALE,

		//////////////////////
		// Vector uniform
		//////////////////////
		UNIFORM_COLOR,

		// Material attributes
		UNIFORM_MATERIAL_AMBIENT, UNIFORM_MATERIAL_DIFFUSE,
		UNIFORM_MATERIAL_SPECULAR,

		// Others...
		UNIFORM_CAMERA_POSITION,

		//////////////////////
		// Boolean uniform
		//////////////////////
		UNIFORM_FLIP,			// animation attributes
		UNIFORM_IS_LIGHT,		// light attributes
		UNIFORM_HIDE_PARTICLE,	// hide particle

		//////////////////////
		// Float uniform
		//////////////////////
		// material attributes
		UNIFORM_MATERIAL_SHININESS,	

		//////////////////////
		// Integer, Enum uniform
		//////////////////////
		UNIFORM_LIGHT_TYPE, UNIFORM_LIGHT_SIZE,

		UNIFORM_EFFECT_TYPE,
		UNIFORM_EFFECT_BLUR_SIZE, UNIFORM_EFFECT_BLUR_AMOUNT,
		UNIFORM_EFFECT_SOBEL, 

		/******************** Light shader ********************/
		//////////////////////
		// Martix uniform
		//////////////////////
		UNIFORM_LIGHT_TRANSLATE, UNIFORM_LIGHT_SCALE, 
		UNIFORM_LIGHT_ROTATE,
		UNIFORM_LIGHT_CAMERA, UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_COLOR,

		UNIFORM_END
	};

public:

	static void SetDrawMode(DrawMode _mode);

private:

	// Private member functions
	static bool	initSDL_GL();
	static void	CloseSDL_GL();
	static void	RegisterUniform();
	static void	InitGLEnvironment();
	static void InitShaders();

	// Private member variables
	static unsigned		m_glArrayMax;
	static DrawMode		m_mode;
	static GLuint		m_vao, m_vbo, m_ebo, /*m_lightVao,*/ m_particleVbo, m_particleEbo;
	static Shaders		m_shaders;
	static GLint		m_uniform[UNIFORM_END];

	// Basic indices and vertices sets
	static const float		m_vertices[192], m_vertices2d[32], m_verticesParticle[96];
	static const unsigned	m_indices[36], m_indices2d[6], m_indicesParticle[18];
	static const int		m_cube, m_rect, m_particle;

	// Locked functions
	GLManager() = delete;
	~GLManager() = delete;
	GLManager(const GLManager& /*_copy*/) = delete;
	void operator=(const GLManager& /*_copy*/) = delete;

};

using GLM = GLManager;

JE_END
