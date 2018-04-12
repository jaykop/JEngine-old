#include "GLManager.h"
#include "NuklearManager.h"
#include "nuklear_sdl_gl3.h"

JE_BEGIN

nk_context* NUKLEAR::m_context = nullptr;
nk_colorf	NUKLEAR::m_background = {0,0,0,0};

void NuklearManager::Init(SDL_Window* _pWindow)
{
	// Init context
	m_context = nk_sdl_init(_pWindow);

	// Load font
	nk_font_atlas *atlas;
	nk_sdl_font_stash_begin(&atlas);
	nk_sdl_font_stash_end();
}

void NuklearManager::EventBegin()
{
	nk_input_begin(m_context);
}

void NuklearManager::EventUpdate(SDL_Event* _event)
{
	nk_input_begin(m_context);
	nk_sdl_handle_event(_event);
	nk_input_end(m_context);
}

void NuklearManager::EventEnd()
{
	nk_input_end(m_context);
}

void NuklearManager::Update(const float /*_dt*/)
{
	static GLsizei s_width, s_height;
	s_width = GLsizei(GLM::m_width);
	s_height = GLsizei(GLM::m_height);
	glViewport(0, 0, s_height, s_height);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(m_background.r, m_background.g, m_background.b, m_background.a);
	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
}

void NuklearManager::Close()
{
	nk_sdl_shutdown();
}

JE_END
