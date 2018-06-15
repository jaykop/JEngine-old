#include "GLManager.h"
#include "Shader.h"
#include "imgui.h"
#include "Vector2.h"

jeBegin

struct jeVertex{
    
    vec3 position;
    vec2 uv;
    vec3 normal;
};

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
float	GLM::m_width = 0;
float	GLM::m_height = 0;
GLint	GLM::m_buffers,
GLM::m_samples,
GLM::m_Attributes;
GLuint	GLM::m_vao[] = { 0 },
GLM::m_vbo[] = { 0 },
GLM::m_ebo[] = { 0 },
GLM::m_fbo = 0,
GLM::m_depthBuffer = 0,
GLM::m_renderTarget = 0;

GLM::Shaders	GLM::m_shader;
GLM::DrawMode	GLM::m_mode = DrawMode::DRAW_FILL;
const GLubyte	*GLM::m_pRenderer = nullptr, *GLM::m_pVendor = nullptr,
		*GLM::m_pVersion = nullptr, *GLM::m_pGlslVersion = nullptr;
unsigned	GLM::m_drawMode = GL_TRIANGLES;

const float GLManager::m_verticesPoint[] =
{	// position				// uv		// normals
        0.f,	0.f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 0.0f, };

const unsigned GLManager::m_indicesPoint[] = { 0 };

const float GLManager::m_verticesPlane[] = {

    // vertic position	// uv		// normals
    -.5f, .5f, 0.f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,	
    .5f, .5f, 0.f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,	
    .5f, -.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,	
    -.5f, -.5f, 0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f	
};

const unsigned GLManager::m_indicesPlane[] = {

    0, 2, 3,
    2, 0, 1	
};

const float GLManager::m_verticesPlane3D[] = {

    // position			// uv		// normals
    -.5f,	.5f,	0.f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		
    .5f,	.5f,	0.f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,	
    .5f,	-.5f,	0.f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,	
    -.5f,	-.5f,	0.f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,	

    0.f,	.5f,	.5f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,	
    0.f,	.5f,	-.5f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,	
    0.f,	-.5f,	-.5f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,	
    0.f,	-.5f,	.5f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,	

    -.5f,	0.f,	-.5f,	1.f, 0.f,	0.0f,  0.0f, 1.0f,		
    .5f,	0.f,	-.5f,	1.f, 1.f,	0.0f,  0.0f, 1.0f,	
    .5f,	0.f,	.5f,	0.f, 1.f,	0.0f,  0.0f, 1.0f,	
    -.5f,	0.f,	.5f,	0.f, 0.f,	0.0f,  0.0f, 1.0f,	

};
const unsigned GLManager::m_indicesPlane3D[] = {
    // front
    0, 2, 3,	
    2, 0, 1,	

    // back
    5, 7, 6,	
    7, 5, 4,	

    // left
    8, 10, 11,	
    10, 8, 9	
};

const float GLManager::m_verticesCube[] =
{
    // front
    -.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  0.0f, 1.0f,	
    .5f,	.5f,	.5f,	.5f, .25f,	0.0f,  0.0f, 1.0f,	
    .5f,	-.5f,	.5f,	.5f, .5f,	0.0f,  0.0f, 1.0f,	
    -.5f,	-.5f,	.5f,	.25f, .5f,	0.0f,  0.0f, 1.0f,	

    // back
    .5f,	.5f,	-.5f,	.75f, .25f,	0.0f,  0.0f, -1.0f,		
    -.5f,	.5f,	-.5f,	1.f, .25f,	0.0f,  0.0f, -1.0f,	
    -.5f,	-.5f,	-.5f,	1.f, .5f,	0.0f,  0.0f, -1.0f,	
    .5f,	-.5f,	-.5f,	.75f, .5f,	0.0f,  0.0f, -1.0f,	

    // left
    -.5f,	.5f,	-.5f,	0.f, .25f,	-1.0f,  0.0f,  0.0f,	
    -.5f,	.5f,	.5f,	.25f, .25f,	-1.0f,  0.0f,  0.0f,	
    -.5f,	-.5f,	.5f,	.25f, .5f,	-1.0f,  0.0f,  0.0f,	
    -.5f,	-.5f,	-.5f,	0.f, .5f,	-1.0f,  0.0f,  0.0f,	

    // right
    .5f,	.5f,	.5f,	.5f, .25f,	1.0f,  0.0f,  0.0f,			
    .5f,	.5f,	-.5f,	.75f, .25f,	1.0f,  0.0f,  0.0f,
    .5f,	-.5f,	-.5f,	.75f, .5f,	1.0f,  0.0f,  0.0f,	
    .5f,	-.5f,	.5f,	.5f, .5f,	1.0f,  0.0f,  0.0f,		

    // down
    -.5f,	-.5f,	.5f,	.25f, .5f,	0.0f, -1.0f,  0.0f,	
    .5f,	-.5f,	.5f,	.5f, .5f,	0.0f, -1.0f,  0.0f,	
    .5f,	-.5f,	-.5f,	.5f, .75f,	0.0f, -1.0f,  0.0f,	
    -.5f,	-.5f,	-.5f,	.25f, .75f,	0.0f, -1.0f,  0.0f,	

    // up
    -.5f,	.5f,	-.5f,	.25f, 0.f,	0.0f,  1.0f,  0.0f,		
    .5f,	.5f,	-.5f,	.5f, 0.f,	0.0f,  1.0f,  0.0f,	
    .5f,	.5f,	.5f,	.5f, .25f,	0.0f,  1.0f,  0.0f,	
    -.5f,	.5f,	.5f,	.25f, .25f,	0.0f,  1.0f,  0.0f	
};

const unsigned GLManager::m_indicesCube[] =
{

    // front
    3, 0, 2,	
    1, 2, 0,

    // back
    6, 7, 5,	
    4, 5, 7,

    // left
    8, 10, 11,
    10, 8, 9,	

    // right
    14, 15, 13,
    12, 13, 15,	

    // down
    16, 18, 19,
    18, 16, 17,

    // up
    20 ,22, 23,	
    22, 20, 21	
};

const float GLManager::m_verticesCone[] = {

    -.5f,	0.f,	0.f,	0.f, 0.f,	0.f,  1.f, 0.f,		
    .5f,	.5f,	-.5f,	0.f, 0.f,	0.f,  1.f, 0.f,		
    .5f,	.5f,	.5f,	0.f, 0.f,	0.f,  1.f, 0.f,		

    -.5f,	0.f,	0.f,	0.f, 0.f,	-1.f,  0.f, 0.f,	
    .5f,	.5f,	.5f,	0.f, 0.f,	-1.f,  0.f, 0.f,	
    .5f,	-.5f,	.5f,	0.f, 0.f,	-1.f,  0.f, 0.f,	

    -.5f,	0.f,	0.f,	0.f, 0.f,	1.f,  0.f, 0.f,		
    .5f,	-.5f,	.5f,	0.f, 0.f,	1.f,  0.f, 0.f,		
    .5f,	-.5f,	-.5f,	0.f, 0.f,	1.f,  0.f, 0.f,		

    -.5f,	0.f,	0.f,	0.f, 0.f,	0.f,  -1.f, 0.f,	
    .5f,	-.5f,	-.5f,	0.f, 0.f,	0.f,  -1.f, 0.f,	
    .5f,	.5f,	-.5f,	0.f, 0.f,	0.f,  -1.f, 0.f,	

    .5f,	.5f,	-.5f,	1.f, 0.f,	0.f,  0.f, -1.f,		
    .5f,	.5f,	.5f,	1.f, 1.f,	0.f,  0.f, -1.f,	
    .5f,	-.5f,	.5f,	0.f, 1.f,	0.f,  0.f, -1.f,	
    .5f,	-.5f,	-.5f,	0.f, 0.f,	0.f,  0.f, -1.f,	
};

const unsigned GLManager::m_indicesCone[] = {

        0, 1,	2,
        3, 4,	5,
        6, 7,	8,
        9, 10,	11,
        15, 14, 12,
        13, 12, 14
};

const unsigned	GLManager::m_elementSize[] = { 1, 6, 72, 144, 6, 18 };
const unsigned	GLManager::m_verticesSize[] = {

	sizeof(GLM::m_verticesPoint), sizeof(GLM::m_verticesPlane),
	sizeof(GLM::m_verticesPlane3D), sizeof(GLM::m_verticesCube),
	sizeof(GLM::m_verticesPlane), sizeof(GLM::m_verticesCone)

};
const unsigned	GLManager::m_indicesSize[] = {

	sizeof(GLM::m_indicesPoint), sizeof(GLM::m_indicesPlane),
	sizeof(GLM::m_verticesPlane3D), sizeof(GLM::m_indicesCube),
	sizeof(GLM::m_indicesPlane),sizeof(GLM::m_indicesCone)

};

//////////////////////////////////////////////////////////////////////////
// GLManager functio bodies
//////////////////////////////////////////////////////////////////////////
bool GLManager::Init()
{
    // force GLEW to use a modern OpenGL method
    glewExperimental = GL_TRUE;

    //Before using shader, initialize glew.
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "*GLManager: Failed to initialize GLEW\n");
        return false;
    }

    // Do gl stuff
    ShowGLVersion();
    InitShaders();
    InitVBO();
    InitFBO();
    InitGLEnvironment();

    return true;
}

void GLManager::Close()
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
    glBufferData(GL_ARRAY_BUFFER, m_verticesSize[SHAPE_PLANE], nullptr, GL_DYNAMIC_DRAW);

    // vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture coordinate position
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // normals of vertices
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

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
        jeDebugPrint("!GLManager - Framebuffer is not created properly.\n");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLManager::InitGLEnvironment()
{
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

void GLManager::DescribeVertex()
{
    glGenVertexArrays(1, &m_testVAO);
    glGenBuffers(1, &m_testVBO);

    glBindVertexArray(m_testVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_testVBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(int), reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(int), reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::uv)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(int), reinterpret_cast<void*>(offsetof(jeVertex, jeVertex::normal)));
    glEnableVertexAttribArray(2);
}

void GLManager::InitShaders()
{
    // Do shader stuff
    for (unsigned i = 0; i < SHADER_END; ++i) {

        m_shader.push_back(new Shader);
        m_shader[i]->CreateShader(Shader::m_vertexShader[i], Shader::VERTEX);
        // TODO
        // Work on geometry shader
        //m_shader[i]->CreateShader(Shader::m_geometryShader[i], Shader::GEOMETRY);
        m_shader[i]->CreateShader(Shader::m_fragmentShader[i], Shader::PIXEL);

        m_shader[i]->CombineShaders();
    }

    jeDebugPrint("*GLManager - Compiled and linked shaders.\n");
}

void GLManager::SetDrawMode(DrawMode _mode)
{
    switch (_mode)
    {
    case DRAW_POINT:
        m_drawMode = GL_POINT;
        glPointSize(5);
        break;
    case DRAW_LINE:
        m_drawMode = GL_LINE;
        break;
    case DRAW_FILL:
        m_drawMode = GL_TRIANGLES;
        break;
    }

    m_mode = _mode;
}

void GLManager::ShowGLVersion()
{
    // Show GL version info
	m_pRenderer = glGetString(GL_RENDERER);
	m_pVendor = glGetString(GL_VENDOR);
	m_pVersion = glGetString(GL_VERSION);
	m_pGlslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    glGetIntegerv(GL_SAMPLE_BUFFERS, &m_buffers);
    glGetIntegerv(GL_SAMPLES, &m_samples);

    jeDebugPrint("*GLManager - GL Vendor: %s / GL Renderer: %s\n", m_pVendor, m_pRenderer);
    jeDebugPrint("*GLManager - GL Version: %s\n", m_pVersion);
    jeDebugPrint("*GLManager - GLSL Version: %s\n", m_pGlslVersion);
    jeDebugPrint("*GLManager - GL Samples: %d / GL Sample Buffers: %d\n", m_samples, m_buffers);

    // Show how much attributes are available
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &m_Attributes);
    jeDebugPrint("*GLManager - Maximum number of vertex attributes supported: %d\n", m_Attributes);
}

void GLManager::Resize(int _width, int _height)
{
    m_width = float(_width);
    m_height = float(_height);
}

void GLManager::SetVAO(GLuint &_vao, GLuint &_vbo, GLuint &_ebo,
    const unsigned _verticeSize, const unsigned _elementSize,
    const float _vertices[], const unsigned _elements[], int _draw)
{
    // Generate vertexy array object
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);

    // Generate vertex buffer object(VBO)
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _verticeSize, _vertices, _draw);

    // Interpret vertex attributes data (s_vertices)
    // vertex position
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    // texture coordinate position
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

    // normals of vertices
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

    // Generate element buffer object
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _elementSize, _elements, _draw);
}

void GLManager::EditorUpdate(const float /*_dt*/)
{
    ImGui::Begin("OpenGL");

    //ImGui::Text("*GL Vendor: %s", m_vendor);
    ImGui::Text("*GL Renderer: %s", m_pRenderer);
    ImGui::Text("*GL Version: %s", m_pVersion);
    /*	ImGui::Text("*GLSL Version: %s", m_glslVersion);
    ImGui::Text("*GL Samples: %d", m_samples);
    ImGui::Text("*GL Sample Buffers: %d", m_buffers);
    ImGui::Text("*Maximum vertex attributes: %d", m_Attributes);*/
    ImGui::Text("*Total Shaders: %d", int(SHADER_END));

    switch (m_mode) {
    case DRAW_FILL:
        ImGui::Text("*Draw Mode: FILL");
        break;
    case DRAW_LINE:
        ImGui::Text("*Draw Mode: LINE");
        break;
    case DRAW_POINT:
        ImGui::Text("*Draw Mode: POINT");
        break;
    }

    if (ImGui::Button("POINT"))
        SetDrawMode(DRAW_POINT);
    ImGui::SameLine();
    if (ImGui::Button("LINE"))
        SetDrawMode(DRAW_LINE);
    ImGui::SameLine();
    if (ImGui::Button("FILL"))
        SetDrawMode(DRAW_FILL);

    ImGui::End();
}

jeEnd

