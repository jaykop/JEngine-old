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
void Object::add_component()
{
	static std::string typeName;
	typeName = ComponentManager::key_to_type(typeid(ComponentType).name());
	auto found = components_.find(typeName);

	DEBUG_ASSERT(found == components_.end(), "Trying to add an existing component!");

	Component* newComponent = ComponentManager::create_component(typeName);
	components_.insert(Components::value_type(typeName, newComponent));
}

template <class ComponentType>
ComponentType* Object::get_component()
{
	static std::string typeName;
	typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);

	DEBUG_ASSERT(found != components_.end(), "No such name of component!");

	return reinterpret_cast<ComponentType*>(found->second);
}

template <class ComponentType>
bool Object::has_component()
{
	static std::string typeName;
	typeName = typeid(ComponentType).name();
	auto found = components_.find(typeName);
	
	return found != components_.end();
}

template <class ComponentType>
void Object::remove_component()
{
	static std::string typeName;
	typeName = ComponentManager::key_to_type(typeid(ComponentType).name());
	auto found = components_.find(typeName);

	DEBUG_ASSERT(found != components_.end(), "No such name of component!");

	delete found->second;
	found->second = nullptr;
}

jeEnd