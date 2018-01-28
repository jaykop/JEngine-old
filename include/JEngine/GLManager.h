#pragma once

#include "glew.h"
#define GLEW_STATIC

#include <vector>
#include "Macro.h"

JE_BEGIN

class GLManager {

	friend class Shader;
	friend class Emitter;
	friend class Application;
	friend class GraphicSystem;

	using Shaders = std::vector<Shader*>;

	enum DrawMode  { DRAW_POINT, DRAW_LINE, DRAW_FILL };
	enum ShaderType  {SHADER_NORMAL, SHADER_LIGHTING, SHADER_PARTICLE, SHADER_SCREEN, SHADER_END};
	enum ShapeType  { SHAPE_POINT, SHAPE_PLANE, SHAPE_PARTICLE, SHAPE_CUBE, SHAPE_END};
	enum UniformType  {

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
		UNIFORM_BILBOARD,

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
		UNIFORM_LIGHT_TRANSLATE, UNIFORM_LIGHT_SCALE, 
		UNIFORM_LIGHT_ROTATE,
		UNIFORM_LIGHT_CAMERA, UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_COLOR,

		/******************** Particle shader ********************/
		UNIFORM_PARTICLE_TRANSLATE, UNIFORM_PARTICLE_SCALE,
		UNIFORM_PARTICLE_ROTATE, UNIFORM_PARTICLE_CAMERA,
		UNIFORM_PARTICLE_PROJECTION,
		
		UNIFORM_PARTICLE_COLOR, UNIFORM_PARTICLE_BILBOARD,
		UNIFORM_PARTICLE_HIDE,			// hide particle

		/******************** Screen shader ********************/
		UNIFORM_SCREEN_FRAMEBUFFER,

		UNIFORM_END
	};

public:

	static void SetDrawMode(DrawMode _mode);

private:

	// Private member functions
	static bool	initSDL_GL(float _width, float _height);
	static void	CloseSDL_GL();
	static void	RegisterUniform();
	//static void CreateGBufferTex(GLenum _texUnit, GLenum _format, GLuint & _texid);
	static void	InitGLEnvironment();
	static void InitVBO();
	static void InitFBO();
	static void InitShaders();
	static void ShowGLVersion();
	static void SetVAO(GLuint &_vao, GLuint &_vbo, GLuint &_ebo,
		const unsigned _verticeSize, const unsigned _elementSize,
		const float _vertices[], const unsigned _elements[]);

	static void SetFBOTexture(GLuint &_buffer);
	static void SetRBOImage(GLuint &_buffer);

	// Private member variables
	static float		m_width, m_height;
	static Shaders		m_shader;
	static DrawMode		m_mode;
	static GLint		m_uniform[UNIFORM_END];
	static GLuint		m_vao[SHAPE_END], m_vbo[SHAPE_END], m_ebo[SHAPE_END];
	static GLuint		m_fbo, m_rbo, m_sampleBuffer, renderedTexture;

	// Basic indices and vertices sets
	static const float		m_verticesPoint[8], m_verticesPlane[32], m_verticesParticle[96], m_verticesCube[192];
	static const unsigned	m_indicesPoint[1], m_indicesPlane[6], m_indicesParticle[18], m_indicesCube[36], 
		m_elementSize[SHAPE_END], m_verticesSize[SHAPE_END], m_indicesSize[SHAPE_END];

	// Locked functions
	GLManager() = delete;
	~GLManager() = delete;
	GLManager(const GLManager& /*_copy*/) = delete;
	void operator=(const GLManager& /*_copy*/) = delete;

};

using GLM = GLManager;

JE_END
