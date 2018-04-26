#pragma once
#include "Object.h"
#include "ObjectContainer.h"

JE_BEGIN

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
