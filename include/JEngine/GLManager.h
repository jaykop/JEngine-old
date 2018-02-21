#pragma once
#include "glew.h"
#define GLEW_STATIC
#include <vector>
#include "Macro.h"

JE_BEGIN

class GLManager {

	friend class Shader;
	friend class Text;
	friend class Model;
	friend class Sprite;
	friend class Emitter;
	friend class Application;
	friend class AssetManager;
	friend class GraphicSystem;
	friend class ImguiManager;

	using Shaders = std::vector<Shader*>;

	enum DrawMode  { DRAW_POINT, DRAW_LINE, DRAW_FILL };
	enum ShaderType  {SHADER_MODEL, SHADER_TEXT, SHADER_LIGHTING, SHADER_PARTICLE, SHADER_SCREEN, SHADER_END};
	enum ShapeType  { SHAPE_POINT, SHAPE_PLANE, SHAPE_PLANE3D, SHAPE_CUBE, SHAPE_TEXT, SHAPE_CONE, SHAPE_END};
	enum UniformType  {

		/******************** Normal shader ********************/
		UNIFORM_TRANSLATE, UNIFORM_SCALE, UNIFORM_ROTATE,
		UNIFORM_CAMERA, UNIFORM_PROJECTION,
		UNIFORM_ANI_TRANSLATE, UNIFORM_ANI_SCALE,
		UNIFORM_COLOR,

		UNIFORM_MATERIAL_AMBIENT, UNIFORM_MATERIAL_DIFFUSE,		// Material attributes
		UNIFORM_MATERIAL_SPECULAR, UNIFORM_MATERIAL_SHININESS,
		UNIFORM_CAMERA_POSITION,
		UNIFORM_FLIP,											// animation attributes
		UNIFORM_IS_LIGHT,										// light attributes
		UNIFORM_BILBOARD,
		UNIFORM_LIGHT_TYPE, UNIFORM_LIGHT_SIZE,

		/******************** Light shader ********************/
		UNIFORM_LIGHT_TRANSLATE, UNIFORM_LIGHT_SCALE, 
		UNIFORM_LIGHT_ROTATEY, UNIFORM_LIGHT_ROTATEZ,
		UNIFORM_LIGHT_CAMERA, UNIFORM_LIGHT_PROJECTION,
		UNIFORM_LIGHT_COLOR,

		/******************** Particle shader ********************/
		UNIFORM_PARTICLE_TRANSLATE, UNIFORM_PARTICLE_SCALE,
		UNIFORM_PARTICLE_ROTATE, UNIFORM_PARTICLE_CAMERA,
		UNIFORM_PARTICLE_PROJECTION,
		
		UNIFORM_PARTICLE_COLOR, UNIFORM_PARTICLE_BILBOARD,
		UNIFORM_PARTICLE_HIDE,			

		/******************** Text shader ********************/
		UNIFORM_TEXT_TRANSLATE, UNIFORM_TEXT_SCALE,
		UNIFORM_TEXT_ROTATE, UNIFORM_TEXT_CAMERA,
		UNIFORM_TEXT_PROJECTION,

		UNIFORM_TEXT_COLOR, UNIFORM_TEXT_BILBOARD,

		/******************** Screen shader ********************/
		UNIFORM_SCREEN_FRAMEBUFFER, UNIFORM_SCREEN_COLOR,
		UNIFORM_SCREEN_EFFECT,
		UNIFORM_SCREEN_BLUR_SIZE, UNIFORM_SCREEN_BLUR_AMOUNT,
		UNIFORM_SCREEN_SOBEL,

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
	static void InitVBO();
	static void InitFBO();
	static void InitShaders();
	static void ShowGLVersion();
	static void Resize(int _width, int _height);
	static void SetVAO(GLuint &_vao, GLuint &_vbo, GLuint &_ebo,
		const unsigned _verticeSize, const unsigned _elementSize,
		const float _vertices[], const unsigned _elements[]);

	static void EditorUpdate(const float _dt);

	// Private member variables
	static float		m_width, m_height;
	static Shaders		m_shader;
	static DrawMode		m_mode;
	static unsigned		m_drawMode;
	static GLint		m_uniform[UNIFORM_END], m_Attributes, m_buffers, m_samples;
	static GLuint		m_vao[SHAPE_END], m_vbo[SHAPE_END], m_ebo[SHAPE_END], m_fbo, m_renderTarget, m_depthBuffer;
	
	static const GLubyte	*m_renderer, *m_vendor, *m_version, *m_glslVersion;

	// Basic indices and vertices sets
	static const float		m_verticesPoint[8], m_verticesPlane[32], m_verticesPlane3D[96], m_verticesCube[192], m_verticesCone[128];
	static const unsigned	m_indicesPoint[1], m_indicesPlane[6], m_indicesPlane3D[18], m_indicesCube[36], m_indicesCone[18],
							m_elementSize[SHAPE_END], m_verticesSize[SHAPE_END], m_indicesSize[SHAPE_END];

	// Locked functions
	GLManager() = delete;
	~GLManager() = delete;
	GLManager(const GLManager& /*_copy*/) = delete;
	void operator=(const GLManager& /*_copy*/) = delete;

};

using GLM = GLManager;

JE_END
