#include <cstdio>
#include <string>
#include "GLManager.h"

JE_BEGIN

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
unsigned GLManager::m_glArraySize = 128;

const float GLManager::m_vertices [] = 
{
	// position				// uv		// normals
	-.5f,	.5f,	0.f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,	// top left	
	.5f,	.5f,	0.f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,	// top right
	.5f,	-.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,	// bottom right
	-.5f,	-.5f,	0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f	// bottom left
};

const int GLManager::m_indices [] = 
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

#ifdef JE_SUPPORT_3D
const float GLManager::m_vertices3D[] = {

	//// position				// uv		// normals
	//-.5f,	.5f,	-.5f,	0.f, 0.f,	.57f,	.57f,	-.57f,	// back top left	
	//.5f,	.5f,	-.5f,	1.f, 0.f,	-.57f,	.57f,	-.57f,	// back top right
	//.5f,	-.5f,	-.5f,	1.f, 1.f,	-.57f,	-.57f,	-.57f,	// back bottom right
	//-.5f,	-.5f,	-.5f,	0.f, 1.f,	.57f,	-.57f,	-.57f,	// back bottom left
	//
	//-.5f,	.5f,	.5f,	0.f, 0.f,	.57f,	.57f,	.57f,	// front top left
	//.5f,	.5f,	.5f,	1.f, 0.f,	-.57f,	.57f,	.57f,	// front top right
	//.5f,	-.5f,	.5f,	1.f, 1.f,	-.57f,	-.57f,	.57f,	// front bottom right
	//-.5f,	-.5f,	.5f,	0.f, 1.f,	.57f,	-.57f, .57f		// front bottom left

	// front
	// position				// uv		// normals
	-.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  0.0f, 1.0f,		// top left	
	.5f,	.5f,	.5f,	.5f, .25f,	0.0f,  0.0f, 1.0f,		// top right
	.5f,	-.5f,	.5f,	.5f, .5f,	0.0f,  0.0f, 1.0f,		// bottom right
	-.5f,	-.5f,	.5f,	.25f, .5f,	0.0f,  0.0f, 1.0f,		// bottom left
		
	// back
	// position				// uv		// normals
	.5f,	.5f,	-.5f,	.75f, .25f,	0.0f,  0.0f, -1.0f,		// top left	
	-.5f,	.5f,	-.5f,	1.f, .25f,	0.0f,  0.0f, -1.0f,		// top right
	-.5f,	-.5f,	-.5f,	1.f, .5f,	0.0f,  0.0f, -1.0f,		// bottom right
	.5f,	-.5f,	-.5f,	.75f, .5f,	0.0f,  0.0f, -1.0f,		// bottom left
	
	// left
	// position				// uv		// normals
	-.5f,	.5f,	-.5f,	0.f, .25f,	-1.0f,  0.0f,  0.0f,	// top left	
	-.5f,	.5f,	.5f,	.25f, .25f,	-1.0f,  0.0f,  0.0f,	// top right
	-.5f,	-.5f,	.5f,	.25f, .5f,	-1.0f,  0.0f,  0.0f,	// bottom right
	-.5f,	-.5f,	-.5f,	0.f, .5f,	-1.0f,  0.0f,  0.0f,	// bottom left
	
	// right
	// position				// uv		// normals
	.5f,	.5f,	.5f,	.5f, .25f,	1.0f,  0.0f,  0.0f,		// top left	
	.5f,	.5f,	-.5f,	.75f, .25f,	1.0f,  0.0f,  0.0f,		// top right
	.5f,	-.5f,	-.5f,	.75f, .5f,	1.0f,  0.0f,  0.0f,		// bottom right
	.5f,	-.5f,	.5f,	.5f, .5f,	1.0f,  0.0f,  0.0f,		// bottom left
	
	// down
	// position				// uv		// normals
	-.5f,	-.5f,	.5f,	.25f, .5f,	0.0f, -1.0f,  0.0f,		// top left	
	.5f,	-.5f,	.5f,	.5f, .5f,	0.0f, -1.0f,  0.0f,		// top right
	.5f,	-.5f,	-.5f,	.5f, .75f,	0.0f, -1.0f,  0.0f,		// bottom right
	-.5f,	-.5f,	-.5f,	.25f, .75f,	0.0f, -1.0f,  0.0f,		// bottom left

	// up
	// position				// uv		// normals
	-.5f,	.5f,	-.5f,	.25f, 0.f,	0.0f,  1.0f,  0.0f,		// top left	
	.5f,	.5f,	-.5f,	.5f, 0.f,	0.0f,  1.0f,  0.0f,		// top right
	.5f,	.5f,	.5f,	.5f, .25f,	0.0f,  1.0f,  0.0f,		// bottom right
	-.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  1.0f,  0.0f		// bottom left
};

const int GLManager::m_indices3D [] = {

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

	//// front
	//0 ,2, 3,	// first triangle
	//2, 0, 1,	// second triangle

	//// back
	//4 ,6, 7,	// first triangle
	//6, 4, 5,	// second triangle

	//// up
	//4 ,1, 0,	// first triangle
	//1, 4, 5,	// second triangle

	//// down
	//3 ,6, 7,	// first triangle
	//6, 3, 2,	// second triangle

	//// left
	//4 ,3, 7,	// first triangle
	//3, 4, 0,	// second triangle

	//// right
	//1 ,6, 2,	// first triangle
	//6, 1, 5	// second triangle

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
#endif

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

		// Generate vertex array object(VAO)
		glGenVertexArrays(1, &m_light_vao);
		// Bind Vertex Array Object
		glBindVertexArray(m_light_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		SetVA();	// Set vertex attributes pointers
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

		// Do gl stuff
		InitGLEnvironment();

		// Do shader stuff
		m_shader[SHADER_LIGHTING].LoadShader(
			"../src/shader/lighting.vs",
			"../src/shader/lighting.fs");

		m_shader[SHADER_NORMAL].LoadShader(
			"../src/shader/normal.vs",
			"../src/shader/normal.fs");

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

	// Set how to draw
	SetDrawMode(m_mode);

	// Active blend function
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef JE_SUPPORT_3D

	glEnable(GL_LESS);
	glEnable(GL_DEPTH_TEST);

#endif

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
	/******************** normal shader ********************/
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_TRANSLATE], "m4_translate");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_SCALE], "m4_scale");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_ROTATE], "m4_rotate");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_CAMERA], "m4_viewport");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_PROJECTION], "m4_projection");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_ANI_TRANSLATE], "m4_aniTranslate");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_ANI_SCALE], "m4_aniScale");

	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_COLOR], "v4_color");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_CAMERA_POSITION], "v3_cameraPosition");
	
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_FLIP], "boolean_flip");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_IS_LIGHT], "boolean_light");

	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_MATERIAL_AMBIENT], "material.m_ambient");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_MATERIAL_DIFFUSE], "material.m_diffuse");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_MATERIAL_SPECULAR], "material.m_specular");
	m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_MATERIAL_SHININESS], "material.m_shininess");

	for (int i = 0; i < m_glArraySize; ++i) {

		std::string index = std::to_string(i);

		std::string color = "v4_lightColor[" + index + "]";
		std::string light = "light[" + index + "].";

		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_COLOR], color.c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_TYPE], light.append("m_type").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_DIFFUSE], light.append("m_diffuse").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_SPECULAR], light.append("m_specular").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_POSITION], light.append("m_position").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_DIRECTION], light.append("m_direction").c_str());

		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_CONST], light.append("m_constant").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_LINEAR], light.append("m_linear").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_QUAD], light.append("m_quadratic").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_CUTOFF], light.append("m_cutOff").c_str());
		m_shader[SHADER_NORMAL].ConnectUniform(m_uniform[UNIFORM_LIGHT_OUTERCUTOFF], light.append("m_outerCutOff").c_str());
	}

	/******************** Light shader ********************/
	m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_LIGHT_TRANSLATE], "m4_translate");
	m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_LIGHT_SCALE], "m4_scale");
	m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_LIGHT_ROTATE], "m4_rotate");
	m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_LIGHT_CAMERA], "m4_viewport");
	m_shader[SHADER_LIGHTING].ConnectUniform(m_uniform[UNIFORM_LIGHT_PROJECTION], "m4_projection");
}

void GLManager::SetVao()
{
	glActiveTexture(GL_TEXTURE0);

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
#ifdef JE_SUPPORT_3D
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices3D), m_vertices3D, GL_STATIC_DRAW);
#else
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_STATIC_DRAW);
#endif
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

	// normals of vertices
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// TODO
	// maybe more...
}

void GLManager::SetEbo()
{
	// Just like vbo...
	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

#ifdef JE_SUPPORT_3D
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices3D), m_indices3D, GL_STATIC_DRAW);
#else
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);
#endif
}

JE_END

