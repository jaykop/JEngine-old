#pragma once
#include "Macro.h"

#ifdef JE_SUPPORT_IMGUI

#include "SDL.h"

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