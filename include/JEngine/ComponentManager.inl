#include <typeinfo>
#include "ComponentManager.h"

JE_BEGIN

template <class ComponentType>
void ComponentManager::RegisterBuilder(
	const char* _componentName, ComponentBuilder* _pBuilder)
{
	static const char* s_name;
	s_name = typeid(ComponentType).name();
	// Check if either there is a existing component builder 
	auto foundName = m_builderMap.find(s_name);

	// If there is existing like that,
	// don't add new builder
	if (foundName != m_builderMap.end())
		JE_DEBUG_PRINT("!Component - No such name of enrolled component: %s\n", s_name);

	// Unless, add new builder
	else {
		m_builderMap.insert(
			BuilderMap::value_type(s_name, _pBuilder));

		// Key = Type name, Value = Class Name
		m_typeMap.insert(
			ComponentTypeMap::value_type(_componentName, typeid(ComponentType).name()));
	}

}

JE_END