#pragma once
#include <vector>
#include "Macro.h"

union SDL_Event;
struct SDL_Window;

JE_BEGIN

class ImguiManager {

	friend class Application;
	friend class StateManager;

	typedef void(*EditorUpdateFunc)(const float);
	using Editors = std::vector<EditorUpdateFunc>;

public:

	static void AddEditorFunc(const EditorUpdateFunc& _pFunc);

private:

	static void Init(SDL_Window* _window);
	static void EventUpdate(SDL_Event* _event);
	static void Update(const float _dt);
	static void Close();

	ImguiManager() = delete;
	~ImguiManager() = delete;
	ImguiManager(const ImguiManager& /*_copy*/) = delete;
	void operator=(const ImguiManager& /*_copy*/) = delete;

	static SDL_Window	*m_pWindow;
	static Editors		m_editors;
};

using IMGUI = ImguiManager;

JE_END