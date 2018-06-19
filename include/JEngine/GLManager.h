#pragma once
#include "glew.h"
#define GLEW_STATIC
#include <vector>
#include "Macro.h"

jeBegin

class GLManager {

    friend class Shader;
    friend class Text;
    friend class Sprite;
    friend class Emitter;
    friend class Application;
    friend class InputHandler;
    friend class AssetManager;
    friend class GraphicSystem;
    friend class ImguiManager;
    friend class NuklearManager;

    using Shaders = std::vector<Shader*>;

    // Locked constuctor, destructor, assign operator
    jeStaticClassDeclaration(GLManager)

    enum DrawMode { DRAW_POINT, DRAW_LINE, DRAW_FILL };
    enum ShaderType { SHADER_MODEL, SHADER_TEXT, SHADER_LIGHTING, SHADER_PARTICLE, SHADER_SCREEN, SHADER_END };
    enum ShapeType { SHAPE_POINT, SHAPE_PLANE, SHAPE_PLANE3D, SHAPE_CUBE, SHAPE_TEXT, SHAPE_CONE, SHAPE_END };

public:

    static void SetDrawMode(DrawMode _mode);

private:

    // Private member functions
    static bool Init();
    static void Close();
    static void InitGLEnvironment();

    static void DescribeVertex();

    static void InitFBO();
    static void InitShaders();
    static void ShowGLVersion();
    static void Resize(int _width, int _height);

    static void EditorUpdate(const float _dt);

    // Private member variables
    static float	m_width, m_height;
    static Shaders	m_shader;
    static DrawMode	m_mode;
    static unsigned	m_drawMode;
    static GLint	m_Attributes, m_buffers, m_samples;
    static GLuint	m_vao[SHAPE_END], m_vbo[SHAPE_END], m_ebo[SHAPE_END], m_fbo, m_renderTarget, m_depthBuffer;

	static std::vector<unsigned> m_planeIndices;
    static const GLubyte *m_pRenderer, *m_pVendor, *m_pVersion, *m_pGlslVersion;

};

using GLM = GLManager;

jeEnd
