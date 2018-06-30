#pragma once
#include "glew.h"
#define GLEW_STATIC
#include <vector>
#include "Macro.h"

jeBegin

class GLManager {

	friend class Mesh;
    friend class Shader;
	friend class Camera;
    friend class Application;
    friend class InputHandler;
    friend class AssetManager;
    friend class GraphicSystem;
    friend class ImguiManager;

    using Shaders = std::vector<Shader*>;

    // Locked constuctor, destructor, assign operator
    jeStaticClassDeclaration(GLManager)

    enum ShaderType { JE_SHADER_MODEL, JE_SHADER_TEXT, JE_SHADER_LIGHTING, JE_SHADER_PARTICLE, JE_SHADER_SCREEN, JE_SHADER_END };
    enum Target { JE_TARGET_SCREEN, JE_TARGET_TEXT, JE_TARGET_END };

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
    static void Resize(int width, int height);

    static void EditorUpdate(float dt);

    // Private member variables
    static float	width_, height_;
    static Shaders	shader_;
    static GLint	attributes_, buffers_, samples_;
    static GLuint	fbo_, texColorBuf_, rbo_;
	static Mesh*	targetMesh_[JE_TARGET_END];

    static const GLubyte *pRenderer_, *pVendor_, *pVersion_, *pGlslVersion_;

};

using GLM = GLManager;

jeEnd
