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

	static bool Init(SDL_Window* pWindow);
	static void EventUpdate(SDL_Event* pEvent);
	static void Update(float dt);
	static void Close();

	static void AddEditorFunc(const EditorUpdateFunc& pFunc);
	static void AddComponentEditor(Component* pComponent);
	static void RemoveComponentEditor(Component* pComponent);
	static void AddObjectEditor(Object* pObject);
	static void RemoveObjectEditor(Object* pObject);
	static void ClearComponentEditor();
	static void ClearObjectEditor();

	static SDL_Window			*pWindow_;
	static EditorList			editors_;
	static ObjectEditorMap		objEditors_;
	static ComponentEditorMap	cptEditors_;
};

using IMGUI = ImguiManager;

jeEnd
