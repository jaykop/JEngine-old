#pragma once

#define NK_INCLUDE_FIXED_TYPES
//#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION

#include "nuklear.h"
#include "Macro.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

union SDL_Event;
struct SDL_Window;

JE_BEGIN

class NuklearManager {

	friend class Application;
	friend class StateManager;

public:

	static nk_context	*m_context;
	static nk_colorf	m_background;

private:

	NuklearManager() = delete;
	~NuklearManager() = delete;
	NuklearManager(const NuklearManager& /*_copy*/) = delete;
	void operator=(const NuklearManager& /*_copy*/) = delete;

	static void Init(SDL_Window* _pWindow);
	static void EventBegin();
	static void EventUpdate(SDL_Event* _event);
	static void EventEnd();
	static void Update(const float _dt);
	static void Close();

};

using NUKLEAR = NuklearManager;

JE_END