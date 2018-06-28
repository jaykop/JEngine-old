#include "GLManager.h"
#include "Shader.h"
#include "Mesh.h"
#include "imgui.h"
#include "GraphicSystem.h"
#include "Mesh.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////

float			GLM::m_width = 0;
float			GLM::m_height = 0;
GLint			GLM::m_buffers, GLM::m_samples, GLM::m_Attributes;
GLuint			GLM::m_vao[] = { 0 }, GLM::m_vbo[] = { 0 }, GLM::m_ebo[] = { 0 }, GLM::m_fbo = 0, GLM::m_depthBuffer = 0, GLM::m_renderTarget = 0;
GLM::Shaders	GLM::m_shader;
GLM::DrawMode	GLM::m_mode = DrawMode::DRAW_FILL;
const GLubyte	*GLM::m_pRenderer = nullptr, *GLM::m_pVendor = nullptr, *GLM::m_pVersion = nullptr, *GLM::m_pGlslVersion = nullptr;
Mesh			*GLM::pMesh_[] = { nullptr };

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
	InitSimplePolygons();
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
		delete pMesh_[index];
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

void GLManager::InitSimplePolygons()
{// Generate vaos, vboa, ebos at once
	glGenVertexArrays(SHAPE_END, m_vao);
	glGenBuffers(SHAPE_END, m_vbo);
	glGenBuffers(SHAPE_END, m_ebo);

	// Describe vertexes and indices
	for (unsigned shape_index = 0; shape_index < SHAPE_CUBE; ++shape_index) {

		switch (shape_index) {
		case SHAPE_TEXT:
		case SHAPE_PLANE:
			pMesh_[shape_index] = Mesh::CreateRect();
			DescribeVertex(m_vao[shape_index], m_vbo[shape_index], 
				m_ebo[shape_index], pMesh_[shape_index]);
			break;
		case SHAPE_CUBE:
			pMesh_[shape_index] = Mesh::CreateCube();
			DescribeVertex(m_vao[shape_index], m_vbo[shape_index],
				m_ebo[shape_index], pMesh_[shape_index]);
			break;
		case SHAPE_PLANE3D:
			pMesh_[shape_index] = Mesh::CreateCrossRect();
			DescribeVertex(m_vao[shape_index], m_vbo[shape_index],
				m_ebo[shape_index], pMesh_[shape_index]);
			break;
		case SHAPE_POINT:
			pMesh_[shape_index] = Mesh::CreatePoint();
			DescribeVertex(m_vao[shape_index], m_vbo[shape_index],
				m_ebo[shape_index], pMesh_[shape_index]);
			break;
		case SHAPE_CONE:
			pMesh_[shape_index] = Mesh::CreateTetrahedron();
			DescribeVertex(m_vao[shape_index], m_vbo[shape_index],
				m_ebo[shape_index], pMesh_[shape_index]);
			break;
		case SHAPE_END:
		default:
			break;
		}

		
	}
}

void GLManager::DescribeVertex(unsigned& vao, unsigned& vbo, unsigned& ebo, Mesh* pMesh)
{
	// Check either if all the objects are initialized
	// and if not, generate them
	if (!vao)
		glGenVertexArrays(1, &vao);
	if (!vbo)
		glGenBuffers(1, &vbo);
	if (!ebo)
		glGenBuffers(1, &ebo);

	// Set vertices and indices vector container
	std::vector<Mesh::jeVertex> vertices;
	std::vector<unsigned> indicies;
	unsigned indiceCount = unsigned(pMesh->GetIndiceCount());
	vertices.reserve(indiceCount);
	indicies.reserve(indiceCount);

	for (unsigned index = 0; index < indiceCount; ++index) {
		Mesh::VertexIndex vi = pMesh->GetIndices().at(index);
		vertices.push_back(Mesh::jeVertex{
			pMesh->GetPoint(vi.a),
			pMesh->GetUV(vi.b),
			pMesh->GetNormal(vi.c) });
		indicies.push_back(index);
	}

	// Decribe the format of vertex and indice
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Mesh::jeVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
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

void GLManager::EditorUpdate(const float /*dt*/)
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

