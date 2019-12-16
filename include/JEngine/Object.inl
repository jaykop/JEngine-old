<<<<<<< HEAD
/******************************************************************************/
/*!
\file   object.inl
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the inline methods of Object class
*/
/******************************************************************************/

#pragma once
#include <object.hpp>
#include <component_manager.hpp>

jeBegin

template <class ComponentType>
void Object::add_component() {

	const char* typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);
	if (found != components_.end()) {
		jeDebugPrint("Trying to add an existing component!");
		return;
	}

	Component* newComponent = ComponentManager::create_component(typeName, this);
	components_.insert(Components::value_type(typeName, newComponent));
}

template <class ComponentType>
ComponentType* Object::get_component() {

	const char* typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);

	if (found == components_.end()) {
		jeDebugPrint("No such name of component!");
		return nullptr;
	}

	return reinterpret_cast<ComponentType*>(found->second);
}

template <class ComponentType>
bool Object::has_component() {

	const char* typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);
	
	return found != components_.end();
}

template <class ComponentType>
void Object::remove_component() {

	const char* typeName = ComponentManager::key_to_type(typeid(ComponentType).name());
	auto found = components_.find(typeName);

	DEBUG_ASSERT(found != components_.end(), "No such name of component!");

	delete found->second;
	found->second = nullptr;
}

jeEnd
=======
#include "Object.h"
#include "ComponentManager.h"
#include "ImguiManager.h"

jeBegin

template<typename ComponentType>
inline void Object::AddComponent()
{
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();
	auto found = componentMap_.find(spComponentName);

	// Found nothing exsting component type
	// Insert new component to the list
	if (found == componentMap_.end()) {
		Component* newComponent = COMPONENT::CreateComponent(spComponentName, this);
		componentMap_.insert(
			ComponentMap::value_type(spComponentName,
				newComponent));
		IMGUI::AddComponentEditor(newComponent);
	}

	else
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
}

template<typename ComponentType>
inline ComponentType* Object::GetComponent()
{
	// Find if there is the one
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();
	auto found = componentMap_.find(spComponentName);

	// If there is return it
	if (found != componentMap_.end())
		// dynamic_cast
		return static_cast<ComponentType*>(found->second);

	else {
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
		return nullptr;
	}
}

template<typename ComponentType>
inline bool Object::HasComponent()
{
	// Find if there is the one
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();
	auto found = componentMap_.find(spComponentName);

	// If there is return it
	if (found != componentMap_.end())
		return true;

	else {
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
		return false;
	}
}

template<typename ComponentType>
inline void Object::RemoveComponent()
{
	// Find if there is the one
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();
	auto found = componentMap_.find(typeid(ComponentType).name());

	// If there is, remove it
	if (found != componentMap_.end()) {
		IMGUI::RemoveComponentEditor(found->second);
		delete found->second;
		found->second = nullptr;
	}

	else
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
}

template<typename ComponentType>
inline void Object::SetGlobalState()
{
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();

	if (!stateMachine_.pGlobalState_) {
		auto found = componentMap_.find(spComponentName);

		// Found same name of component,
		// then put that to global state
		if (found != componentMap_.end())
			stateMachine_.pGlobalState_ = (CustomComponent*)found->second;

		else
			jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
	}

	else
		jeDebugPrint("!Object - There is an allocated global state already: %s\n", spComponentName);
}

template<typename ComponentType>
inline void Object::SetCurrentState()
{
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();

	if (!stateMachine_.pCurrentState_) {

		auto found = componentMap_.find(spComponentName);

		// Found same name of component,
		// then put that to current state
		if (found != componentMap_.end())
			stateMachine_.pCurrentState_ = (CustomComponent*)found->second;

		else
			jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
	}

	else
		jeDebugPrint("!Object - There is an allocated current state already: %s\n", spComponentName);
}

template<typename ComponentType>
inline void Object::ChangeState()
{
	static const char* spComponentName;
	spComponentName = typeid(ComponentType).name();
	auto found = componentMap_.find(spComponentName);

	// Found same component name,
	// switch to that state.
	if (found != componentMap_.end()) {
		stateMachine_.pPreviousState_ = stateMachine_.pCurrentState_;
		stateMachine_.pPreviousState_->Close();
		stateMachine_.pPreviousState_->Unload();
		stateMachine_.pCurrentState_ = (CustomComponent*)found->second;
		stateMachine_.pCurrentState_->Init();
	}

	else
		jeDebugPrint("!Object - No such name of enrolled component: %s\n", spComponentName);
}

jeEnd
>>>>>>> 4af9948ac99f35dbd94753136ac865176a80e124
