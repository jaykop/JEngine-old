#include "Object.h"
#include "Component.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "ComponentManager.h"

JE_BEGIN

Object::Object(const char* _name)
	:m_name(_name), m_active(true), m_pParent(nullptr),
	m_id(ObjectFactory::m_registerNumber)
{}

Object::~Object()
{
	ClearChildren();
	ClearComponents();
}

unsigned Object::GetId() const
{
	return m_id;
}

const std::string& Object::GetName(void) const
{
	return m_name;
}

void Object::SetName(const char* _name)
{
	m_name.assign(_name);
}

void Object::AddChild(Object* _child)
{
	// Find if there is the once
	auto found = m_childObjs.find(_child->GetName());
	
	// If there is not, add
	if (found == m_childObjs.end()) {
		m_childObjs.insert(
			ChildObjects::value_type(
				_child->GetName(), _child));
		
		_child->SetParent(this);
	}

	// Unless...
	else
		JE_DEBUG_PRINT("Existing child\n");
}

void Object::RemoveChild(const char* _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is, remove
	if (found != m_childObjs.end()) {
		m_childObjs.erase(_name);			// Remove from the child list
		m_pOBC->RemoveObject(_name);	// Remove from obj manager
	}
		
	else 
		JE_DEBUG_PRINT("No such object\n");
}

Object* Object::GetChild(const char* _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is return the one
	if (found != m_childObjs.end())
		return found->second;

	// Unless...
	else {
		JE_DEBUG_PRINT("No such object\n");
		return nullptr;
	}
}

bool Object::HasChild(const char* _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is return the one
	if (found != m_childObjs.end())
		return true;

	// Unless...
	else {
		JE_DEBUG_PRINT("No such object\n");
		return false;
	}
}

void Object::SetParent(Object* _pObject)
{
	m_pParent = _pObject;
}

Object* Object::GetParent()
{
	return m_pParent;
}

bool Object::HasParent()
{
	return m_pParent != nullptr ? true : false ;
}

void Object::SetActive(bool _active)
{
	m_active = _active;
}

bool Object::GetActive(void) const
{
	return m_active;
}

void Object::ClearComponents()
{
	// Clear all components in the list
	for (auto component : m_componentMap) {

		if (component.second) {
			delete component.second;
			component.second = nullptr;
		}
	}
}

void Object::ClearChildren()
{
	for (auto child : m_childObjs)
		m_pOBC->RemoveObject(child.second->GetName().c_str());
}


JE_END
