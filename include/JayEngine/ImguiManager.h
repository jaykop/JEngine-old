#pragma once
#include "Macro.h"
#include "SDL2\SDL.h"

struct SDL_Window;

NS_JE_BEGIN

class ImguiManager {

public:

	static void Init(SDL_Window* _window);
	static void Update();
	static void Close();

private:

	ImguiManager() {};
	~ImguiManager() {};
	ImguiManager(const ImguiManager& /*_copy*/) {};
	void operator=(const ImguiManager& /*_copy*/) {};

	static SDL_Window* m_pWindow;
};

NS_JE_END