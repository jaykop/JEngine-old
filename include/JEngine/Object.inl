#include "Object.h"
#include "ComponentManager.h"
#include "ImguiManager.h"

JE_BEGIN

template<typename ComponentType>
inline void Object::AddComponent()
{
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(s_componentName);

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == m_componentMap.end()) {
		Component* newComponent = COMPONENT::CreateComponent<ComponentType>(this);
		m_componentMap.insert(
			ComponentMap::value_type(s_componentName,
				newComponent));
		IMGUI::AddComponentEditor(newComponent);
	}

	else
		JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
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
		JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
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
		JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
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
		JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
}

template<typename ComponentType>
inline void Object::SetGlobalState()
{
	if (!m_StateMachine.m_pGlobalState) {
		static const char* s_componentName;
		s_componentName = typeid(ComponentType).name();
		auto found = m_componentMap.find(s_componentName);

		// Found nothing exsting component type
		// Insert new component to the list
		if (found == m_componentMap.end())
			m_StateMachine.m_pGlobalState = JE_CREATE_CUSTOM_COMPONENT(ComponentType, this);

		else
			JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
	}

	else
		JE_DEBUG_PRINT("!Object - There is an allocated global state already: %s\n", s_componentName);
}

template<typename ComponentType>
inline void Object::SetCurretState()
{
	if (!m_StateMachine.m_pCurrentState) {

		static const char* s_componentName;
		s_componentName = typeid(ComponentType).name();
		auto found = m_componentMap.find(s_componentName);

		// Found nothing exsting component type
		// Insert new component to the list
		if (found == m_componentMap.end())
			m_StateMachine.m_pCurrentState = JE_CREATE_CUSTOM_COMPONENT(ComponentType, this);

		else
			JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
	}

	else
		JE_DEBUG_PRINT("!Object - There is an allocated current state already: %s\n", s_componentName);
}

template<typename ComponentType>
inline void Object::ChangeState()
{
	static const char* s_componentName;
	s_componentName = typeid(ComponentType).name();
	auto found = m_componentMap.find(s_componentName);

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == m_componentMap.end()) {
		m_StateMachine.m_pPreviousState = m_StateMachine.m_pCurrentState;
		m_StateMachine.m_pPreviousState->Close();
		m_StateMachine.m_pPreviousState->Unload();
		m_StateMachine.m_pCurrentState = JE_CREATE_CUSTOM_COMPONENT(ComponentType, this);
		m_StateMachine.m_pCurrentState->Init();
	}

	else
		JE_DEBUG_PRINT("!Object - No such name of enrolled component: %s\n", s_componentName);
}

JE_END
