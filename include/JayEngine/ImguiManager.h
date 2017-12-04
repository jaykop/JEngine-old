#pragma once
#include "Macro.h"

#ifdef IMGUI

#include "SDL2\SDL.h"

struct SDL_Window;

JE_BEGIN

class ImguiManager {

public:

	static void Init(SDL_Window* _window);
	static void Update();
	static void Close();

private:

	ImguiManager() {};
	~ImguiManager() {};
	ImguiManager(const ImguiManager& /*_copy*/) = delete;
	void operator=(const ImguiManager& /*_copy*/) = delete;

	static SDL_Window* m_pWindow;
};

using IMGUI = ImguiManager;

JE_END

#endif