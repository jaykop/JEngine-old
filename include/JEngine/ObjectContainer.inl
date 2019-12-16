#pragma once
#include "Object.h"
#include "ObjectContainer.h"

jeBegin

template<typename ComponentType>
inline ObjectMap ObjectContainer::GetObjects()
{
	ObjectMap container;

	for (auto object : objectMap_) {
		if (object.second->HasComponent<ComponentType>())
			container.insert(
				ObjectMap::value_type(
					object.second->m_name.c_str(), object.second));
	}
	
	return container;
}

jeEnd
