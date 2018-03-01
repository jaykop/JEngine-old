#pragma once
#include <vector>
#include <unordered_map>
#include "Macro.h"

union SDL_Event;
struct SDL_Window;

JE_BEGIN

class Object;

typedef void(*EditorUpdateFunc)(const float);
using ComponentEditorMap = std::unordered_map<std::string, EditorUpdateFunc>;
using ObjectEditorMap = std::unordered_map<std::string, ComponentEditorMap* >;

class ImguiManager {

	friend class Application;
	friend class StateManager;

	using EditorList = std::vector<EditorUpdateFunc>;

public:

	static void AddEditorFunc(const EditorUpdateFunc& _pFunc);
	static void AddObjectEditorFunc(Object* _object);

private:

	static void Init(SDL_Window* _window);
	static void EventUpdate(SDL_Event* _event);
	static void Update(const float _dt);
	static void Close();

	ImguiManager() = delete;
	~ImguiManager() = delete;
	ImguiManager(const ImguiManager& /*_copy*/) = delete;
	void operator=(const ImguiManager& /*_copy*/) = delete;

	static SDL_Window		*m_pWindow;
	static EditorList		m_editors;
	static ObjectEditorMap	m_objEditor;
};

using IMGUI = ImguiManager;

JE_END