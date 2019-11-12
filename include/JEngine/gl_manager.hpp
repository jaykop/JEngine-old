/******************************************************************************/
/*!
\file   gl_manager.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/15(yy/mm/dd)

\description
Contains the definition of GLManager class
*/
/******************************************************************************/
#pragma once

#define GLEW_STATIC
#include <glew.h>
#include <macros.hpp>
#include <vector>

union SDL_Event;
struct SDL_Window;

jeBegin

class Shader;
class GLManager
{
	friend class Application;
	friend class SceneManager;

	using Shaders = std::vector<Shader*>;

	enum ShaderType { JE_SHADER_MODEL, JE_SHADER_TEXT, JE_SHADER_LIGHTING, JE_SHADER_PARTICLE, JE_SHADER_SCREEN, JE_SHADER_END };
	enum Target { JE_TARGET_SCREEN, JE_TARGET_TEXT, JE_TARGET_END };

public:

	static float get_width();
	static float get_height();

private:

	static void initialize(float w, float h);
	static void update(const SDL_Event& event);
	static void close();

	static void initialize_shaders();
	static void initialize_framebuffer(); 

	GLManager() = delete;
	~GLManager() = delete;
	GLManager(GLManager&&) = delete;
	GLManager(const GLManager&) = delete;
	GLManager& operator= (GLManager&&) = delete;
	GLManager& operator= (const GLManager&) = delete;

	static float width_, height_;
	static GLint buffers_, samples_, attributes_;
	static Shaders shader_;
};

jeEnd