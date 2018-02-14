#include "GLManager.h"
#include "Shader.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
float				GLManager::m_width = 0;
float				GLManager::m_height = 0;
GLint				GLManager::m_uniform[] = { 0 };
GLuint				GLManager::m_vao[] = { 0 };
GLuint				GLManager::m_vbo[] = { 0 };
GLuint				GLManager::m_ebo[] = { 0 };
GLuint				GLManager::m_fbo = 0;
GLuint				GLManager::m_depthBuffer = 0;
GLuint				GLManager::m_renderTarget = 0;
GLManager::Shaders	GLManager::m_shader;
GLManager::DrawMode GLManager::m_mode = DrawMode::DRAW_FILL;

// TODO
// For test...
GLuint				GLManager::m_deferredFBO = 0;
GLuint				GLManager::m_positionBuffer = 0;
GLuint				GLManager::m_normalBuffer = 0;
GLuint				GLManager::m_colorBuffer = 0;
GLuint				GLManager::m_passIndex[] = { 0 };
GLuint				GLManager::m_passIndex1 = 0;
GLuint				GLManager::m_passIndex2 = 0;

const float GLManager::m_verticesPoint[] = 
{	// position				// uv		// normals
	0.f,	0.f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 0.0f,};

const unsigned GLManager::m_indicesPoint[] = { 0 };

const float GLManager::m_verticesPlane[] = {

	// vertic position	// uv		// normals
	-.5f, .5f, 0.f,		0.f, 0.f,	0.0f,  0.0f, 1.0f,	// top left	
	.5f, .5f, 0.f,		1.f, 0.f,	0.0f,  0.0f, 1.0f,	// top right
	.5f, -.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,	// bottom right
	-.5f, -.5f, 0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f	// bottom left
};

const unsigned GLManager::m_indicesPlane[] = {

	/***************/
	/*  *   second */
	/*     *       */
	/*        *    */
	/*  first    * */
	/***************/

	// front
	0, 2, 3,	// first triangle
	2, 0, 1		// second triangle
};

const float GLManager::m_verticesPlane3D[] = {

	// position				// uv		// normals
	-.5f,	.5f,	0.f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	-.5f,	0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	-.5f,	0.f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,		// bottom left

	0.f,	.5f,	.5f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	0.f,	.5f,	-.5f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	0.f,	-.5f,	-.5f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	0.f,	-.5f,	.5f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,		// bottom left

	-.5f,	0.f,	-.5f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	0.f,	-.5f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	0.f,	.5f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	0.f,	.5f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,		// bottom left

};
const unsigned GLManager::m_indicesPlane3D[] = {
	// front
	0, 2, 3,	// first triangle
	2, 0, 1,	// second triangle

	// back
	5, 7, 6,	// first triangle
	7, 5, 4,	// second triangle

	// left
	8, 10, 11,	// first triangle
	10, 8, 9	// second triangle
};

const float GLManager::m_verticesCube [] = 
{
	// front
	// position				// uv		// normals
	-.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	.5f,	.5f,	.5f, .25f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	-.5f,	.5f,	.5f, .5f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	-.5f,	.5f,	.25f, .5f,	0.0f,  0.0f, 1.0f,		// bottom left

	// back
	.5f,	.5f,	-.5f,	.75f, .25f,	0.0f,  0.0f, -1.0f,		// top left	
	-.5f,	.5f,	-.5f,	1.f, .25f,	0.0f,  0.0f, -1.0f,		// top right
	-.5f,	-.5f,	-.5f,	1.f, .5f,	0.0f,  0.0f, -1.0f,		// bottom right
	.5f,	-.5f,	-.5f,	.75f, .5f,	0.0f,  0.0f, -1.0f,		// bottom left

	// left
	-.5f,	.5f,	-.5f,	0.f, .25f,	-1.0f,  0.0f,  0.0f,	// top left	
	-.5f,	.5f,	.5f,	.25f, .25f,	-1.0f,  0.0f,  0.0f,	// top right
	-.5f,	-.5f,	.5f,	.25f, .5f,	-1.0f,  0.0f,  0.0f,	// bottom right
	-.5f,	-.5f,	-.5f,	0.f, .5f,	-1.0f,  0.0f,  0.0f,	// bottom left

	// right
	.5f,	.5f,	.5f,	.5f, .25f,	1.0f,  0.0f,  0.0f,		// top left	
	.5f,	.5f,	-.5f,	.75f, .25f,	1.0f,  0.0f,  0.0f,		// top right
	.5f,	-.5f,	-.5f,	.75f, .5f,	1.0f,  0.0f,  0.0f,		// bottom right
	.5f,	-.5f,	.5f,	.5f, .5f,	1.0f,  0.0f,  0.0f,		// bottom left

	// down
	-.5f,	-.5f,	.5f,	.25f, .5f,	0.0f, -1.0f,  0.0f,		// top left	
	.5f,	-.5f,	.5f,	.5f, .5f,	0.0f, -1.0f,  0.0f,		// top right
	.5f,	-.5f,	-.5f,	.5f, .75f,	0.0f, -1.0f,  0.0f,		// bottom right
	-.5f,	-.5f,	-.5f,	.25f, .75f,	0.0f, -1.0f,  0.0f,		// bottom left

	// up
	-.5f,	.5f,	-.5f,	.25f, 0.f,	0.0f,  1.0f,  0.0f,		// top left	
	.5f,	.5f,	-.5f,	.5f, 0.f,	0.0f,  1.0f,  0.0f,		// top right
	.5f,	.5f,	.5f,	.5f, .25f,	0.0f,  1.0f,  0.0f,		// bottom right
	-.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  1.0f,  0.0f		// bottom left
};

const unsigned GLManager::m_indicesCube [] =
{
	//				 4					5
	//				  ****************
	//			  *				 *	 */
	//		0 *				 *		 */
	//		/***************/ 1		 */
	//		/*  *   second */		 */
	//		/*     *       */		 */
	//		/*        *    */		 */ 6
	//		/*  first    * */	 *
	//		/***************/*
	//		3				2

	// front
	0, 2, 3,	// first triangle
	2, 0, 1,	// second triangle

	// back
	5, 7, 6,	// first triangle
	7, 5, 4,	// second triangle

	// left
	8, 10, 11,	// first triangle
	10, 8, 9,	// second triangle

	// right
	12, 14, 15,	// first triangle
	14, 12, 13,	// second triangle

	// down
	16, 18, 19,	// first triangle
	18, 16, 17,	// second triangle

	// up
	20 ,22, 23,	// first triangle
	22, 20, 21	// second triangle
};

const float GLManager::m_verticesCone[] = {

	// position				// uv		// normals
	-.5f,	0.f,	0.f,	0.f, 0.f,	0.f,  1.f, 0.f,		// front
	.5f,	.5f,	-.5f,	0.f, 0.f,	0.f,  1.f, 0.f,		// front
	.5f,	.5f,	.5f,	0.f, 0.f,	0.f,  1.f, 0.f,		// front

	-.5f,	0.f,	0.f,	0.f, 0.f,	-1.f,  0.f, 0.f,	// right
	.5f,	.5f,	.5f,	0.f, 0.f,	-1.f,  0.f, 0.f,	// right
	.5f,	-.5f,	.5f,	0.f, 0.f,	-1.f,  0.f, 0.f,	// right

	-.5f,	0.f,	0.f,	0.f, 0.f,	1.f,  0.f, 0.f,		// left
	.5f,	-.5f,	.5f,	0.f, 0.f,	1.f,  0.f, 0.f,		// left
	.5f,	-.5f,	-.5f,	0.f, 0.f,	1.f,  0.f, 0.f,		// left

	-.5f,	0.f,	0.f,	0.f, 0.f,	0.f,  -1.f, 0.f,	// back
	.5f,	-.5f,	-.5f,	0.f, 0.f,	0.f,  -1.f, 0.f,	// back
	.5f,	.5f,	-.5f,	0.f, 0.f,	0.f,  -1.f, 0.f,	// back

	.5f,	.5f,	-.5f,	1.f, 0.f,	0.f,  0.f, -1.f,	// botom - top left	
	.5f,	.5f,	.5f,	1.f, 1.f,	0.f,  0.f, -1.f,	// botom - top right
	.5f,	-.5f,	.5f,	0.f, 1.f,	0.f,  0.f, -1.f,	// botom - bottom right
	.5f,	-.5f,	-.5f,	0.f, 0.f,	0.f,  0.f, -1.f,	// botom - bottom left
};

const unsigned GLManager::m_indicesCone[] = {

	0,	1,	2,
	3,	4,	5,
	6,	7,	8,
	9,	10,	11,
	15, 14, 12,
	13, 12, 14 
};

const unsigned		GLManager::m_elementSize[] = { 1, 6, 72, 144, 6, 18 };
const unsigned		GLManager::m_verticesSize[] = { 

	sizeof(GLM::m_verticesPoint), sizeof(GLM::m_verticesPlane), 
	sizeof(GLM::m_verticesPlane3D), sizeof(GLM::m_verticesCube),
	sizeof(GLM::m_verticesPlane), sizeof(GLM::m_verticesCone)

};
const unsigned		GLManager::m_indicesSize[] = { 

	sizeof(GLM::m_indicesPoint), sizeof(GLM::m_indicesPlane),
	sizeof(GLM::m_verticesPlane3D), sizeof(GLM::m_indicesCube),
	sizeof(GLM::m_indicesPlane),sizeof(GLM::m_indicesCone)

};

//////////////////////////////////////////////////////////////////////////
// GLManager functio bodies
//////////////////////////////////////////////////////////////////////////
bool GLManager::initSDL_GL(float _width, float _height)
{
	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	//Before using shader, initialize glew.
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "*GLManager: Failed to initialize GLEW\n");
		return false;
	}

	// Unless
	else {

		// Set wdith and heught
		m_width		= _width;
		m_height	= _height;

		// Do gl stuff
		ShowGLVersion();
		InitShaders();
		InitVBO();
		InitFBO();				// These two are to be off to visualise deferred rendering
		InitGLEnvironment();	// These two are to be off to visualise deferred rendering
		//InitDefferedFBO();
		RegisterUniform();
	}

	return true;
}

void GLManager::CloseSDL_GL()
{
	// Clear shaders
	for (auto shader : m_shader) {
		delete shader;
		shader = nullptr;
	}

	m_shader.clear();

	for (int index = 0; index < SHAPE_END; ++index) {
		glDeleteBuffers(1, &m_ebo[index]);
		glDeleteBuffers(1, &m_vbo[index]);
		glDeleteVertexArrays(1, &m_vao[index]);
	}

	glDeleteFramebuffers(1, &m_fbo);
}

void GLManager::InitVBO()
{
	glActiveTexture(GL_TEXTURE0);

	SetVAO(m_vao[SHAPE_POINT], m_vbo[SHAPE_POINT], m_ebo[SHAPE_POINT],
		m_verticesSize[SHAPE_POINT], m_indicesSize[SHAPE_POINT],
		m_verticesPoint, m_indicesPoint);

	SetVAO(m_vao[SHAPE_PLANE], m_vbo[SHAPE_PLANE], m_ebo[SHAPE_PLANE],
		m_verticesSize[SHAPE_PLANE], m_indicesSize[SHAPE_PLANE],
		m_verticesPlane, m_indicesPlane);

	SetVAO(m_vao[SHAPE_PLANE3D], m_vbo[SHAPE_PLANE3D], m_ebo[SHAPE_PLANE3D],
		m_verticesSize[SHAPE_PLANE3D], m_indicesSize[SHAPE_PLANE3D],
		m_verticesPlane3D, m_indicesPlane3D);

	SetVAO(m_vao[SHAPE_CUBE], m_vbo[SHAPE_CUBE], m_ebo[SHAPE_CUBE],
		m_verticesSize[SHAPE_CUBE], m_indicesSize[SHAPE_CUBE],
		m_verticesCube, m_indicesCube);

	SetVAO(m_vao[SHAPE_CONE], m_vbo[SHAPE_CONE], m_ebo[SHAPE_CONE],
		m_verticesSize[SHAPE_CONE], m_indicesSize[SHAPE_CONE],
		m_verticesCone, m_indicesCone);

	// Set vao for text
	// Generate vertexy array object
	glGenVertexArrays(1, &m_vao[SHAPE_TEXT]);
	glBindVertexArray(m_vao[SHAPE_TEXT]);

	// Generate vertex buffer object(VBO)
	glGenBuffers(1, &m_vbo[SHAPE_TEXT]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[SHAPE_TEXT]);

	glBindVertexArray(0);
}

void GLManager::InitFBO()
{
	// Create and bind the FBO
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	// The texture we're going to render to
	glGenTextures(1, &m_renderTarget);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_renderTarget);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLsizei(m_width), GLsizei(m_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GLsizei(m_width), GLsizei(m_height));
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTarget, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
		JE_DEBUG_PRINT("!GLManager - Framebuffer is not created properly.\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void GLManager::InitDefferedFBO()
{
	// Create and bind the FBO
	glGenFramebuffers(1, &m_deferredFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_deferredFBO);

	// The depth buffer
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GLsizei(m_width), GLsizei(m_height));

	// Create the textures for position, normal and color
	CreateGBufferTexture(GL_TEXTURE0, GL_RGB32F, m_positionBuffer);		// Position
	CreateGBufferTexture(GL_TEXTURE1, GL_RGB8, m_colorBuffer);			// Color
	CreateGBufferTexture(GL_TEXTURE2, GL_RGB32F, m_normalBuffer);		// Normal

	// Attach the textures to the framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_positionBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_colorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_normalBuffer, 0);

	GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(4, drawBuffers);

	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
		JE_DEBUG_PRINT("!GLManager - Framebuffer is not created properly.\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_passIndex1 = glGetSubroutineIndex(m_shader[SHADER_DEFERRED]->m_programId, GL_FRAGMENT_SHADER, "render1");
	m_passIndex2 = glGetSubroutineIndex(m_shader[SHADER_DEFERRED]->m_programId, GL_FRAGMENT_SHADER, "render2");
}

void GLManager::InitGLEnvironment()
{
	// Show how much attributes are available
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	JE_DEBUG_PRINT("*GLManager - Maximum nr of vertex attributes supported: %d\n", nrAttributes);

	// Set how to draw
	SetDrawMode(m_mode);

	// Set depth 
	glEnable(GL_LEQUAL);
	glDisable(GL_DEPTH_TEST);

	//Set blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Set antialiasing/multisampling
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glDisable(GL_MULTISAMPLE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

}

void GLManager::InitShaders() 
{
	// Do shader stuff
	for (unsigned i = 0; i < SHADER_END; ++i) 
		m_shader.push_back(new Shader);

	m_shader[SHADER_MODEL]->LoadShader(
		"../src/JEngine/shader/model.vs",
		"../src/JEngine/shader/model.fs");

	m_shader[SHADER_TEXT]->LoadShader(
		"../src/JEngine/shader/text.vs",
		"../src/JEngine/shader/text.fs");

	m_shader[SHADER_LIGHTING]->LoadShader(
		"../src/JEngine/shader/lighting.vs",
		"../src/JEngine/shader/lighting.fs");

	m_shader[SHADER_PARTICLE]->LoadShader(
		"../src/JEngine/shader/particle.vs",
		"../src/JEngine/shader/particle.fs");

	m_shader[SHADER_SCREEN]->LoadShader(
		"../src/JEngine/shader/screen.vs",
		"../src/JEngine/shader/screen.fs");

	m_shader[SHADER_DEFERRED]->LoadShader(
		"../src/JEngine/shader/deferred.vs",
		"../src/JEngine/shader/deferred.fs");
}

void GLManager::SetDrawMode(DrawMode _mode)
{
	switch (_mode)
	{
	case DRAW_POINT:
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case DRAW_LINE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case DRAW_FILL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	m_mode = _mode;
}

void GLManager::RegisterUniform()
{
	/******************** normal shader ********************/
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_TRANSLATE, "m4_translate");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_SCALE, "m4_scale");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_ROTATE, "m4_rotate");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_CAMERA, "m4_viewport");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_PROJECTION, "m4_projection");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_ANI_TRANSLATE, "m4_aniTranslate");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_ANI_SCALE, "m4_aniScale");

	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_COLOR, "v4_color");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_CAMERA_POSITION, "v3_cameraPosition");
	
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_FLIP, "boolean_flip");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_IS_LIGHT, "boolean_light");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_BILBOARD, "boolean_bilboard");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_LIGHT_SIZE, "int_lightSize");

	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_MATERIAL_AMBIENT, "material.m_ambient");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_MATERIAL_DIFFUSE, "material.m_diffuse");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_MATERIAL_SPECULAR, "material.m_specular");
	m_shader[SHADER_MODEL]->ConnectUniform(UNIFORM_MATERIAL_SHININESS, "material.m_shininess");

	/******************** Light shader ********************/
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_TRANSLATE, "m4_translate");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_SCALE, "m4_scale");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_CAMERA, "m4_viewport");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_PROJECTION, "m4_projection");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_COLOR, "v4_color");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_ROTATEY, "m4_rotateY");
	m_shader[SHADER_LIGHTING]->ConnectUniform(UNIFORM_LIGHT_ROTATEZ, "m4_rotateZ");

	/******************** Particle shader ********************/
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_COLOR, "v4_color");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_TRANSLATE, "m4_translate");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_SCALE, "m4_scale");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_ROTATE, "m4_rotate");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_CAMERA, "m4_viewport");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_PROJECTION, "m4_projection");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_HIDE, "boolean_hide");
	m_shader[SHADER_PARTICLE]->ConnectUniform(UNIFORM_PARTICLE_BILBOARD, "boolean_bilboard");

	/******************** Text shader ********************/
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_COLOR, "v4_color");
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_TRANSLATE, "m4_translate");
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_SCALE, "m4_scale");
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_ROTATE, "m4_rotate");
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_CAMERA, "m4_viewport");
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_PROJECTION, "m4_projection");
	m_shader[SHADER_TEXT]->ConnectUniform(UNIFORM_TEXT_BILBOARD, "boolean_bilboard");

	/******************** Screen shader ********************/
	m_shader[SHADER_SCREEN]->ConnectUniform(UNIFORM_SCREEN_COLOR, "v4_screenColor");
	m_shader[SHADER_SCREEN]->ConnectUniform(UNIFORM_SCREEN_FRAMEBUFFER, "Framebuffer");
	m_shader[SHADER_SCREEN]->ConnectUniform(UNIFORM_SCREEN_EFFECT, "enum_effectType");
	m_shader[SHADER_SCREEN]->ConnectUniform(UNIFORM_SCREEN_BLUR_SIZE, "float_blurSize");
	m_shader[SHADER_SCREEN]->ConnectUniform(UNIFORM_SCREEN_BLUR_AMOUNT, "float_blurAmount");
	m_shader[SHADER_SCREEN]->ConnectUniform(UNIFORM_SCREEN_SOBEL, "float_sobelAmount");
	
	/******************** Deferred shader ********************/
	m_shader[SHADER_DEFERRED]->ConnectUniform(UNIFORM_DEFERRED_TRANSLATE, "m4_translate");
	m_shader[SHADER_DEFERRED]->ConnectUniform(UNIFORM_DEFERRED_SCALE, "m4_scale");
	m_shader[SHADER_DEFERRED]->ConnectUniform(UNIFORM_DEFERRED_ROTATE, "m4_rotate");
	m_shader[SHADER_DEFERRED]->ConnectUniform(UNIFORM_DEFERRED_CAMERA, "m4_viewport");
	m_shader[SHADER_DEFERRED]->ConnectUniform(UNIFORM_DEFERRED_PROJECTION, "m4_projection");
}

void GLManager::ShowGLVersion()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint buffers = 0, samples = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
	glGetIntegerv(GL_SAMPLES, &samples);

	JE_DEBUG_PRINT("*GLManager - GL Vendor: %s\n", vendor);
	JE_DEBUG_PRINT("*GLManager - GL Renderer: %s\n", renderer);
	JE_DEBUG_PRINT("*GLManager - GL Version: %s\n", version);
	JE_DEBUG_PRINT("*GLManager - GLSL Version: %s\n", glslVersion);
	JE_DEBUG_PRINT("*GLManager - GL Samples: %d\n", samples);
	JE_DEBUG_PRINT("*GLManager - GL Sample Buffers: %d\n", buffers);
}

void GLManager::SetVAO(GLuint &_vao, GLuint &_vbo, GLuint &_ebo,
	const unsigned _verticeSize, const unsigned _elementSize,
	const float _vertices[], const unsigned _elements[])
{
	// Generate vertexy array object
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Generate vertex buffer object(VBO)
	glGenBuffers(1, &_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, _verticeSize, _vertices, GL_STATIC_DRAW);

	// Interpret vertex attributes data (s_vertices)
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// texture coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// normals of vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Generate element buffer object
	glGenBuffers(1, &_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elementSize, _elements, GL_STATIC_DRAW);
}

void GLManager::CreateGBufferTexture(GLenum _texUnit, GLenum _format, GLuint &_texid)
{
	glActiveTexture(_texUnit);
	glGenTextures(1, &_texid);
	glBindTexture(GL_TEXTURE_2D, _texid);

	//glTexImage2D(GL_TEXTURE_2D, 0, _format, GLsizei(m_width), GLsizei(m_height), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexStorage2D(GL_TEXTURE_2D, 1, _format, GLsizei(m_width), GLsizei(m_height));

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
}

JE_END

