#pragma once
#include <vector>
#include "Macro.h"

union SDL_Event;
struct SDL_Window;

jeBegin

class Component;

typedef void(*EditorUpdateFunc)(const float);

class ImguiManager {

	friend class Object;
	friend class ObjectFactory;
	friend class ObjectContainer;
	friend class Application;
	friend class StateManager;

	using EditorList		 = std::vector<EditorUpdateFunc>;
	using ComponentEditorMap = std::vector<Component*>;
	using ObjectEditorMap	 = std::vector<Object*>;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(ImguiManager)

private:

	static bool Init(SDL_Window* _window);
	static void EventUpdate(SDL_Event* _event);
	static void Update(float dt);
	static void Close();

	static void AddEditorFunc(const EditorUpdateFunc& _pFunc);
	static void AddComponentEditor(Component* _component);
	static void RemoveComponentEditor(Component* _component);
	static void AddObjectEditor(Object* _object);
	static void RemoveObjectEditor(Object* _object);
	static void ClearComponentEditor();
	static void ClearObjectEditor();

	static SDL_Window			*pWindow_;
	static EditorList			m_editors;
	static ObjectEditorMap		m_objEditors;
	static ComponentEditorMap	m_cptEditors;
};

using IMGUI = ImguiManager;

jeEnd
