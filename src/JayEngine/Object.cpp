#include "Object.h"
#include "ObjectManager.h"
#include "ComponentManager.h"

NS_JE_BEGIN

Object::Object(const std::string& _name)
	:m_name(_name), m_active(true), m_pParent(nullptr)
{
}

Object::~Object()
{
	// Remove all child objects belong to this object
	for (auto child : m_childObjs)
		ObjectManager::RemoveObject(child.second->GetName().c_str());

	// Remove all components belong to this object
	m_cptContainer.ClearContainer();
}

const std::string& Object::GetName(void) const
{
	return m_name;
}

void Object::SetName(const char * _name)
{
	m_name.assign(_name);
}

void Object::AddChild(Object* _child)
{
	// Find if there is the once
	auto found = m_childObjs.find(_child->GetName());
	
	// If there is not, add
	if (found == m_childObjs.end())
		m_childObjs.insert(
			ChildObjects::value_type(
				_child->GetName(), _child));

	// Unless...
	else
		JE_DEBUG_PRINT("Existing child\n");
}

void Object::RemoveChild(const char * _name)
{
	// Find if there is the once
	auto found = m_childObjs.find(_name);

	// If there is, remove
	if (found != m_childObjs.end()) {
		m_childObjs.erase(_name);			// Remove from the child list
		ObjectManager::RemoveObject(_name);	// Remove from obj manager
	}
		
	// Unless...
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

void Object::SetActive(bool _active)
{
	m_active = _active;
}

bool Object::GetActive(void) const
{
	return m_active;
}

NS_JE_END