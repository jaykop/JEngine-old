#pragma once
#include "Object.h"
#include "ObjectContainer.h"

NS_JE_BEGIN

template<typename ComponentType>
inline ObjectMap ObjectContainer::GetObjects()
{
	ObjectMap container;

	for (auto object : m_objectMap) {
		if (object.second->HasComponent<ComponentType>())
			container.insert(
				ObjectMap::value_type(
					object.second->GetName().c_str(), object.second));
	}
	
	return container;
}

NS_JE_END