#include "GLManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "imgui.h"
#include "GraphicSystem.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////

std::vector<GraphicSystem::jeVertex> planeVertexes{
	{vec3(-.5f, .5f, 0.f), vec2(0.f, 0.f), vec3(0, 0, 1.f) },
{ vec3(.5f, .5f, 0.f), vec2(1.f, 0.f), vec3(0, 0, 1.f) },
{ vec3(.5f, -.5f, 0.f), vec2(1.f, 1.f), vec3(0, 0, 1.f) },
{ vec3(-.5f, -.5f, 0.f), vec2(0.f, 1.f), vec3(0, 0, 1.f) }
};

float	GLM::m_width = 0;
float	GLM::m_height = 0;
GLint	GLM::m_buffers, GLM::m_samples, GLM::m_Attributes;
GLuint	GLM::m_vao[] = { 0 }, GLM::m_vbo[] = { 0 }, GLM::m_ebo[] = { 0 }, GLM::m_fbo = 0, GLM::m_depthBuffer = 0, GLM::m_renderTarget = 0;
GLM::Shaders	GLM::m_shader;
GLM::DrawMode	GLM::m_mode = DrawMode::DRAW_FILL;
const GLubyte	*GLM::m_pRenderer = nullptr, *GLM::m_pVendor = nullptr, *GLM::m_pVersion = nullptr, *GLM::m_pGlslVersion = nullptr;
std::vector<unsigned> GLM::m_planeIndices{ 0, 2, 3, 2, 0, 1 };

//////////////////////////////////////////////////////////////////////////
// GLManager functio bodies
//////////////////////////////////////////////////////////////////////////
bool GLManager::Init()
{
    // force GLEW to use a modern OpenGL method
    glewExperimental = GL_TRUE;

    //Before using shader, initialize glew.
    if (glewInit() != GLEW_OK) {
        jeDebugPrint("!GLManager - Failed to initialize GLEW\n");
        return false;
    }

    // Do gl stuff
    ShowGLVersion();
    InitShaders();
	DescribeVertex();
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

	// Describe plane attributes for fianl render screen 
	glBindVertexArray(m_vao[SHAPE_PLANE]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[SHAPE_PLANE]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GraphicSystem::jeVertex) * planeVertexes.size(),
		static_cast<const void*>(&planeVertexes[0]), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicSystem::jeVertex), 
		reinterpret_cast<void*>(offsetof(GraphicSystem::jeVertex, jeVertex::position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GraphicSystem::jeVertex), 
		reinterpret_cast<void*>(offsetof(GraphicSystem::jeVertex, jeVertex::uv)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicSystem::jeVertex), 
		reinterpret_cast<void*>(offsetof(GraphicSystem::jeVertex, jeVertex::normal)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo[SHAPE_PLANE]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * m_planeIndices.size(),
		static_cast<const void*>(&m_planeIndices[0]), GL_DYNAMIC_DRAW);
}

void GLManager::InitGLEnvironment()
{

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
    // Generate vaos, vboa, ebos at once
    glGenVertexArrays(SHAPE_END, m_vao);
    glGenBuffers(SHAPE_END, m_vbo);
    glGenBuffers(SHAPE_END, m_ebo);

    // Describe vertexes and indices
	for (unsigned index = 0; index < SHAPE_END; ++index) {
		glBindVertexArray(m_vao[index]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo[index]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicSystem::jeVertex),
			reinterpret_cast<void*>(offsetof(GraphicSystem::jeVertex, jeVertex::position)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GraphicSystem::jeVertex),
			reinterpret_cast<void*>(offsetof(GraphicSystem::jeVertex, jeVertex::uv)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GraphicSystem::jeVertex),
			reinterpret_cast<void*>(offsetof(GraphicSystem::jeVertex, jeVertex::normal)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}

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

    ImGui::End();
}

jeEnd

