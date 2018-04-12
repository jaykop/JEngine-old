#pragma once
#include "Object.h"
#include "ObjectContainer.h"

JE_BEGIN

template<typename ComponentType>
inline void ObjectContainer::AddComponent(const char* _fromObject)
{
	GetObject(_fromObject)->AddComponent(typeid(ComponentType).name());
}

template<typename ComponentType>
inline ComponentType* ObjectContainer::GetComponent(const char* _fromObject)
{
	return GetObject(_fromObject)->GetComponent(typeid(ComponentType).name());
}

template<typename ComponentType>
inline bool ObjectContainer::HasComponent(const char* _fromObject)
{
	return GetObject(_fromObject)->HasComponent(typeid(ComponentType).name());
}

template<typename ComponentType>
inline void ObjectContainer::RemoveComponent(const char* _fromObject)
{
	GetObject(_fromObject)->RemoveComponent(typeid(ComponentType).name());
}

template<typename ComponentType>
inline ObjectMap ObjectContainer::GetObjects()
{
	ObjectMap container;

	for (auto object : m_objectMap) {
		if (object.second->HasComponent<ComponentType>())
			container.insert(
				ObjectMap::value_type(
					object.second->m_name.c_str(), object.second));
	}
	
	return container;
}

JE_END
