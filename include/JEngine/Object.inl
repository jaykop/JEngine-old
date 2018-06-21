#include "Object.h"
#include "ComponentManager.h"
#include "ImguiManager.h"

jeBegin

template<typename ComponentType>
inline void Object::AddComponent()
{
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(s_componentName);

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == m_componentMap.end()) {
		Component* newComponent = COMPONENT::CreateComponent(s_componentName, this);
		m_componentMap.insert(
			ComponentMap::value_type(s_componentName,
				newComponent));
		IMGUI::AddComponentEditor(newComponent);
	}

	else
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
}

template<typename ComponentType>
inline ComponentType* Object::GetComponent()
{
	// Find if there is the one
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(s_componentName);

	// If there is return it
	if (found != m_componentMap.end())
		// dynamic_cast
		return static_cast<ComponentType*>(found->second);

	else {
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
		return nullptr;
	}
}

template<typename ComponentType>
inline bool Object::HasComponent()
{
	// Find if there is the one
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(s_componentName);

	// If there is return it
	if (found != m_componentMap.end())
		return true;

	else {
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
		return false;
	}
}

template<typename ComponentType>
inline void Object::RemoveComponent()
{
	// Find if there is the one
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(typeid(ComponentType).name());

	// If there is, remove it
	if (found != m_componentMap.end()) {
		IMGUI::RemoveComponentEditor(found->second);
		delete found->second;
		found->second = nullptr;
	}

	else
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
}

template<typename ComponentType>
inline void Object::SetGlobalState()
{
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();

	if (!m_StateMachine.m_pGlobalState) {
		auto found = m_componentMap.find(s_componentName);

		// Found same name of component,
		// then put that to global state
		if (found != m_componentMap.end())
			m_StateMachine.m_pGlobalState = (CustomComponent*)found->second;

		else
			jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
	}

	else
		jeDebugPrint("!Object - There is an allocated global state already: %s\n", s_componentName);
}

template<typename ComponentType>
inline void Object::SetCurrentState()
{
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();

	if (!m_StateMachine.m_pCurrentState) {

		auto found = m_componentMap.find(s_componentName);

		// Found same name of component,
		// then put that to current state
		if (found != m_componentMap.end())
			m_StateMachine.m_pCurrentState = (CustomComponent*)found->second;

		else
			jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
	}

	else
		jeDebugPrint("!Object - There is an allocated current state already: %s\n", s_componentName);
}

template<typename ComponentType>
inline void Object::ChangeState()
{
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(s_componentName);

	// Found same component name,
	// switch to that state.
	if (found != m_componentMap.end()) {
		m_StateMachine.m_pPreviousState = m_StateMachine.m_pCurrentState;
		m_StateMachine.m_pPreviousState->Close();
		m_StateMachine.m_pPreviousState->Unload();
		m_StateMachine.m_pCurrentState = (CustomComponent*)found->second;
		m_StateMachine.m_pCurrentState->Init();
	}

	else
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", s_componentName);
}

jeEnd
