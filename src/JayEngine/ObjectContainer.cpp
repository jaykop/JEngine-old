#include "Object.h"
#include "ObjectContainer.h"
#include "ComponentManager.h"

JE_BEGIN

ObjectContainer::~ObjectContainer()
{
	ClearObjectMap();
}

void ObjectContainer::RemoveObject(const char* _name)
{
	// Find the one to remove
	auto toRemove = m_objectMap.find(_name);

	// If found the one
	if (toRemove != m_objectMap.end()) {
		delete (*toRemove).second;
	}

	else
		JE_DEBUG_PRINT("No such object\n");
}

Object* ObjectContainer::GetObject(const char * _name)
{
	// Find the one to remove
	auto toRemove = m_objectMap.find(_name);

	// If found the one
	if (toRemove != m_objectMap.end())
		return (*toRemove).second;

	else {
		JE_DEBUG_PRINT("No such object\n");
		return nullptr;
	}
}

ObjectMap* ObjectContainer::GetObjectMap()
{
	return &m_objectMap;
}

bool ObjectContainer::HasObject(const char* _name)
{
	// Find the one to remove
	auto toRemove = m_objectMap.find(_name);

	// If found the one
	if (toRemove != m_objectMap.end()) {
		return true;
	}

	return false;
}

void ObjectContainer::ClearObjectMap()
{
	for (auto obj : m_objectMap)
	{
		if (obj.second) {
			delete obj.second;
			obj.second = nullptr;
		}
	}

	m_objectMap.clear();
}

JE_END

