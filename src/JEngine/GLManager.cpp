#include "GLManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "imgui.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////

float			GLM::width_ = 0;
float			GLM::height_ = 0;
GLint			GLM::buffers_, GLM::samples_, GLM::attributes_;
GLuint			GLM::fbo_ = 0, GLM::rbo_ = 0, GLM::texColorBuf_ = 0;
GLM::Shaders	GLM::shader_;
const GLubyte	*GLM::pRenderer_ = nullptr, *GLM::pVendor_ = nullptr, *GLM::pVersion_ = nullptr, *GLM::pGlslVersion_ = nullptr;
Mesh			*GLM::targetMesh_[] = { nullptr };

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
	InitFramebuffer();
    InitGLEnvironment();

    return true;
}

void GLManager::Close()
{
    // Clear shaders
    for (auto shader : shader_) {
        delete shader;
        shader = nullptr;
    }

    shader_.clear();

    for (int index = 0; index < JE_TARGET_END; ++index)
		delete targetMesh_[index];

    glDeleteFramebuffers(1, &fbo_);
}

void GLManager::InitFramebuffer()
{
	// Int target render meshes for screen and text
	targetMesh_[JE_TARGET_TEXT] = Mesh::CreateRect();
	targetMesh_[JE_TARGET_TEXT]->builtIn_ = true;
	GLM::DescribeVertex(targetMesh_[JE_TARGET_TEXT]);

	targetMesh_[JE_TARGET_SCREEN] = Mesh::CreateRect();
	targetMesh_[JE_TARGET_SCREEN]->builtIn_ = true;
	GLM::DescribeVertex(targetMesh_[JE_TARGET_SCREEN]);

    // Create and bind the FBO
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

    // The texture we're going to render to
    glGenTextures(1, &texColorBuf_);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, texColorBuf_);

    // Give an empty image to OpenGL ( the last "0" )
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLsizei(width_), GLsizei(height_), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texColorBuf_, 0);

    // The depth buffer
    glGenRenderbuffers(1, &rbo_);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GLsizei(width_), GLsizei(height_));
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_);

    // Always check that our framebuffer is ok
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
        jeDebugPrint("!GLManager - Framebuffer is not created properly.\n");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void GLManager::InitShaders()
{
    // Do shader stuff
    for (unsigned i = 0; i < JE_SHADER_END; ++i) {

        shader_.push_back(new Shader);
        shader_[i]->CreateShader(Shader::vertexShader_[i], Shader::JE_VERTEX);
        // TODO
        // Work on geometry shader
        //shader_[i]->CreateShader(Shader::m_geometryShader[i], Shader::JE_GEOMETRY);
        shader_[i]->CreateShader(Shader::fragmentShader_[i], Shader::JE_PIXEL);

        shader_[i]->CombineShaders();
    }

    jeDebugPrint("*GLManager - Compiled and linked shaders.\n");
}

void GLManager::ShowGLVersion()
{
    // Show GL version info
    pRenderer_ = glGetString(GL_RENDERER);
    pVendor_ = glGetString(GL_VENDOR);
    pVersion_ = glGetString(GL_VERSION);
    pGlslVersion_ = glGetString(GL_SHADING_LANGUAGE_VERSION);

    glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers_);
    glGetIntegerv(GL_SAMPLES, &samples_);

    jeDebugPrint("*GLManager - GL Vendor: %s / GL Renderer: %s\n", pVendor_, pRenderer_);
    jeDebugPrint("*GLManager - GL Version: %s\n", pVersion_);
    jeDebugPrint("*GLManager - GLSL Version: %s\n", pGlslVersion_);
    jeDebugPrint("*GLManager - GL Samples: %d / GL Sample Buffers: %d\n", samples_, buffers_);

    // Show how much attributes are available
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributes_);
    jeDebugPrint("*GLManager - Maximum number of vertex attributes supported: %d\n", attributes_);
}

void GLManager::Resize(int width, int height)
{
    width_ = float(width);
    height_ = float(height);
}

void GLManager::EditorUpdate(const float /*dt*/)
{
    ImGui::Begin("OpenGL");

    //ImGui::Text("*GL Vendor: %s", m_vendor);
    ImGui::Text("*GL Renderer: %s", pRenderer_);
    ImGui::Text("*GL Version: %s", pVersion_);
    /*	ImGui::Text("*GLSL Version: %s", m_glslVersion);
    ImGui::Text("*GL Samples: %d", samples_);
    ImGui::Text("*GL Sample Buffers: %d", buffers_);
    ImGui::Text("*Maximum vertex attributes: %d", attributes_);*/
    ImGui::Text("*Total Shaders: %d", int(JE_SHADER_END));

    ImGui::End();
}

void GLManager::DescribeVertex(Mesh* pMesh)
{
	// Check either if all the objects are initialized
	// and if not, generate them
	if (!pMesh->m_vao)
		glGenVertexArrays(1, &pMesh->m_vao);
	if (!pMesh->m_vbo)
		glGenBuffers(1, &pMesh->m_vbo);
	if (!pMesh->m_ebo)
		glGenBuffers(1, &pMesh->m_ebo);

	// Set vertices and indices vector container
	std::vector<Mesh::jeVertex> vertices;
	std::vector<unsigned> indicies;
	unsigned indiceCount = unsigned(pMesh->GetIndiceCount());
	vertices.reserve(indiceCount);
	indicies.reserve(indiceCount);

	for (unsigned index = 0; index < indiceCount; ++index) {
		Mesh::jeIndex vi = pMesh->GetIndices().at(index);
		vertices.push_back(Mesh::jeVertex{
			pMesh->GetPoint(vi.a),
			pMesh->GetUV(vi.b),
			pMesh->GetNormal(vi.c) });
		indicies.push_back(index);
	}

	// Decribe the format of vertex and indice
	glBindVertexArray(pMesh->m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, pMesh->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::jeVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indicies.size(), &indicies[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::jeVertex),
		reinterpret_cast<void*>(offsetof(Mesh::jeVertex, jeVertex::position)));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Mesh::jeVertex),
		reinterpret_cast<void*>(offsetof(Mesh::jeVertex, jeVertex::uv)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Mesh::jeVertex),
		reinterpret_cast<void*>(offsetof(Mesh::jeVertex, jeVertex::normal)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

jeEnd

