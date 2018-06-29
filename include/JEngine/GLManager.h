#pragma once
#include "glew.h"
#define GLEW_STATIC
#include <vector>
#include "Macro.h"

jeBegin

class GLManager {

	friend class Mesh;
    friend class Shader;
    friend class Text;
    friend class Model;
	friend class Light;
    friend class Emitter;
	friend class Camera;
    friend class Application;
    friend class InputHandler;
    friend class AssetManager;
    friend class GraphicSystem;
    friend class ImguiManager;

    using Shaders = std::vector<Shader*>;

    // Locked constuctor, destructor, assign operator
    jeStaticClassDeclaration(GLManager)

    enum ShaderType { SHADER_MODEL, SHADER_TEXT, SHADER_LIGHTING, SHADER_PARTICLE, SHADER_SCREEN, SHADER_END };
    enum Target { TARGET_SCREEN, TARGET_TEXT, TARGET_END };

public:

private:

    // Private member functions
    static bool Init();
    static void Close();

	static void InitFramebuffer();
    static void InitGLEnvironment();
    static void InitShaders();
    static void ShowGLVersion();

	static void	DescribeVertex(Mesh* pMesh);
    static void Resize(int _width, int _height);

    static void EditorUpdate(float dt);

    // Private member variables
    static float	m_width, m_height;
    static Shaders	m_shader;
    static GLint	m_Attributes, m_buffers, m_samples;
    static GLuint	m_fbo, m_renderTarget, m_depthBuffer;
	static Mesh*	targetMesh_[TARGET_END];

    static const GLubyte *m_pRenderer, *m_pVendor, *m_pVersion, *m_pGlslVersion;

};

using GLM = GLManager;

jeEnd
