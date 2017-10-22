#include <cstdio>
#include "GLManager.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
GLuint GLManager::m_vao = 0;
GLuint GLManager::m_vbo = 0;
GLuint GLManager::m_ebo = 0;
GLuint GLManager::m_light_vao = 0;
Shader GLManager::m_shader[];
GLint GLManager::m_uniform[];
GLManager::DrawMode GLManager::m_mode = DrawMode::DRAW_FILL;

const float GLManager::s_vertices [] = 
{
	// position				// uv		
	-.5f,	.5f,	0.f,	0.f, 0.f,	// top left	
	.5f,	.5f,	0.f,	1.f, 0.f,	// top right
	.5f,	-.5f,	0.f,	1.f, 1.f,	// bottom right
	-.5f,	-.5f,	0.f,	0.f, 1.f	// bottom left
};

const int GLManager::s_indices [] = 
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

const float GLManager::s_vertices3D[] = {

	// position				// uv		// normals
	-.5f,	.5f,	-.5f,	0.f, 0.f,	.57f,	.57f,	-.57f,		// back top left	
	.5f,	.5f,	-.5f,	1.f, 0.f,	-.57f,	.57f,	-.57f,		// back top right
	.5f,	-.5f,	-.5f,	1.f, 1.f,	-.57f,	-.57f,	-.57f,		// back bottom right
	-.5f,	-.5f,	-.5f,	0.f, 1.f,	.57f,	-.57f,	-.57f,		// back bottom left

	-.5f,	.5f,	.5f,	0.f, 0.f,	.57f,	.57f,	.57f,		// front top left
	.5f,	.5f,	.5f,	1.f, 0.f,	-.57f,	.57f,	.57f,		// front top right
	.5f,	-.5f,	.5f,	1.f, 1.f,	-.57f,	-.57f,	.57f,		// front bottom right
	-.5f,	-.5f,	.5f,	0.f, 1.f,	.57f,	-.57f, .57f		// front bottom left
};

const int GLManager::s_indices3D [] = {

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
	0 ,2, 3,	// first triangle
	2, 0, 1,	// second triangle

	// back
	4 ,6, 7,	// first triangle
	6, 4, 5,	// second triangle

	// up
	4 ,1, 0,	// first triangle
	1, 4, 5,	// second triangle

	// down
	3 ,6, 7,	// first triangle
	6, 3, 2,	// second triangle

	// left
	4 ,3, 7,	// first triangle
	3, 4, 0,	// second triangle

	// right
	1 ,6, 2,	// first triangle
	6, 1, 5		// second triangle
};

//////////////////////////////////////////////////////////////////////////
// GLManager functio bodies
//////////////////////////////////////////////////////////////////////////
bool GLManager::initSDL_GL()
{
	// force GLEW to use a modern OpenGL method
	glewExperimental = GL_TRUE;

	//Before using shader, initialize glew.
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	// Unless
	else {
		SetVao();	// Generate vertex array object
		SetVbo();	// Generate vertex buffuer object
		SetVA();	// Set vertex attributes pointers
		SetEbo();	// Set indices

		// Do gl stuff
		InitGLEnvironment();

		// Do shader stuff
		m_shader[SHADER_NORMAL].LoadShader(
			"../src/resource/shader/vertexshader.vs",
			"../src/resource/shader/fragmentshader.fs");

		//m_shader[SHADER_LIGHTING].LoadShader(
		//	"../src/resource/shader/lighting.vs",
		//	"../src/resource/shader/lighting.fs");

		RegisterUniform();
	}

	return true;
}

void GLManager::CloseSDL_GL()
{
}

void GLManager::InitGLEnvironment()
{
	// Show how much attributes are available
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	JE_DEBUG_PRINT("Maximum nr of vertex attributes supported: %d\n", nrAttributes);

	// Active blend function
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Texture attribute setting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

void GLManager::SetDrawMode(DrawMode _mode)
{
	switch (_mode)
	{
	case DRAW_DOT:
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
	// For normal shader
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_TRANSLATE], "m4_translate");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_SCALE], "m4_scale");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_ROTATE], "m4_rotate");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_CAMERA], "m4_viewport");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_PROJECTION], "m4_projection");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_ANI_TRANSLATE], "m4_aniTranslate");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_ANI_SCALE], "m4_aniScale");

	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_COLOR], "v4_color");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_COLOR], "v4_lightColor");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_POSITION], "v3_lightPosition");
	
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_FLIP], "boolean_flip");

	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_TRANSLATE_LIGHT], "m4_translate");
	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_SCALE_LIGHT], "m4_scale");
	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_ROTATE_LIGHT], "m4_rotate");
	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_CAMERA_LIGHT], "m4_viewport");
	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_PROJECTION_LIGHT], "m4_projection");
	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_COLOR_LIGHT], "v4_lightColor");
	//m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_COLOR_OBJECT], "v4_objectColor");

}

void GLManager::SetVao()
{
	// TODO
	//glActiveTexture(GL_TEXTURE0);

	// Generate vertex array object(VAO)
	glGenVertexArrays(1, &m_vao);

	// Bind Vertex Array Object
	glBindVertexArray(m_vao);

}

void GLManager::SetVbo()
{
	// Generate vertex buffer object(VBO)
	glGenBuffers(1, &m_vbo);

	// Set vbo active
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Now we can copy vertex data to this vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices3D), s_vertices3D, GL_STATIC_DRAW);
}

void GLManager::SetVA()
{
	// Interpret vertex attributes data (s_vertices)
	// vertex position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// text coordinate position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// text coordinate position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// TODO
	// maybe more...
}

void GLManager::SetEbo()
{
	// Just like vbo...
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(s_indices3D), s_indices3D, GL_STATIC_DRAW);
}

NS_JE_END

